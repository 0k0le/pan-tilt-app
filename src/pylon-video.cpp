/*
 * Matthew Todd Geiger
 * pylon-video.cpp
 */

// Local includes
#include "macros.hpp"
#include "pylon-video.hpp"

#include <opencv2/opencv.hpp>

using namespace Pylon;
using namespace GenApi;
using namespace cv;

// Private statics
uint8_t Recorder::_imageBuffer[RESX * RESY * BYTES_PER_PIXEL];
std::mutex Recorder::_mtx;
std::mutex Recorder::_closeThreadMtx;
bool Recorder::_closeThread;

Recorder::Recorder(const char* const cameraSerial, int framesPerSecond, int quality) {
    _fps = framesPerSecond;
    _qual = quality;

    M_PRINT("Initializing Pylon/Basler camera API");
    InitializeCamera(cameraSerial); // Initialize the camera on startup
}

Recorder::~Recorder() {
    M_PRINT("Shutting down recorder");

    // This needs to be first to prevent early deletion
    if(_recordThread != nullptr) {
        M_PRINT("Waiting for recorder thread");
        
        _closeThreadMtx.lock(); // Send close message to recorder thread
        _closeThread = true;
        _closeThreadMtx.unlock();

        _recordThread->join(); // Wait for thread to close
        delete _recordThread;
    }

    if(_camera != nullptr) {
        M_PRINT("Closing camera handle");
        _camera->Close(); // Close pylon camera object
        delete _camera;
    }

    // Cleanup pylon API
    M_PRINT("Cleanup pylon API");
    PylonTerminate();
}

DeviceInfoList_t::const_iterator Recorder::FindCamera(const char* const cameraSerial, CTlFactory& TlFactory, DeviceInfoList_t& lstDevices) {
    TlFactory.EnumerateDevices(lstDevices); // Get list of devices on network

    DeviceInfoList_t::const_iterator it; // iterate through devices to find one that matches serial number
    if(!lstDevices.empty()) {
        for(it = lstDevices.begin(); it != lstDevices.end(); ++it) {
            M_PRINT("Camera found on network: %s", it->GetSerialNumber().c_str());
            if(strcmp(it->GetSerialNumber().c_str(), cameraSerial) == 0) {
                return it;
            }
        }
    }

    return nullptr;
}

void Recorder::InitializeCamera(const char* const cameraSerial) {
    // Initialize Pylon API
    PylonInitialize();
    
    _closeThread = false;

    CTlFactory& TlFactory = CTlFactory::GetInstance(); // User transport layer factory to communicate with basler devices over transport layer
    DeviceInfoList_t lstDevices;

    try {
        // Connect to camera
        M_PRINT("Locating camera: %s", cameraSerial);
        
        DeviceInfoList_t::const_iterator it = FindCamera(cameraSerial, TlFactory, lstDevices); // Find camera on network

        if(it == nullptr)
            throw GENERIC_EXCEPTION("Failed to find camera with serial: %s", cameraSerial);

        M_PRINT("Connecting to: %s", cameraSerial);
        _camera = new CMyInstantCamera(TlFactory.CreateDevice(*it)); // Create camera device
        M_PRINT("Camera connection successful");

        _camera->Open(); // Open camera

        // Configure camera
        M_PRINT("Configuring camera settings");
        
        // Get handle on camera properties
        CIntegerParameter width(_camera->GetNodeMap(), "Width");
        CIntegerParameter height(_camera->GetNodeMap(), "Height");
        CIntegerParameter offsetX(_camera->GetNodeMap(), "OffsetX");
        CIntegerParameter offsetY(_camera->GetNodeMap(), "OffsetY");
        CEnumParameter pixelFormat(_camera->GetNodeMap(), "PixelFormat");

        // Set camera properties
        //width.TrySetValue(RESX, IntegerValueCorrection_Nearest);
        //height.TrySetValue(RESY, IntegerValueCorrection_Nearest);
        width.TrySetToMaximum(); // Set to maximum, we will downscale using opencv
        height.TrySetToMaximum();
        offsetX.TrySetToMinimum();
        offsetY.TrySetToMinimum();
        pixelFormat.SetIntValue(PixelType_RGB8packed);

        _threadInfo.camera = _camera;

    } catch (const GenericException& e) {
        M_FATAL("An exception occured. %s", e.GetDescription());
    }

    // return
}

void Recorder::RecordThread(void *data) {
    RThreadData *baslerInfo = (RThreadData *)data;

    CIntegerParameter width(baslerInfo->camera->GetNodeMap(), "Width");
    CIntegerParameter height(baslerInfo->camera->GetNodeMap(), "Height");

    M_PRINT("Camera X: %ld -- Camera Y: %ld", width.GetValue(), height.GetValue());
    CLock& lock = baslerInfo->camera->RetLock();

    M_PRINT("Initiating grabber");
    lock.Lock();
    baslerInfo->camera->StartGrabbing(); // Start grabbing frames from camera over network
    lock.Unlock();

    CGrabResultPtr ptrGrabResult;

    while(1) {
        lock.Lock();
        if(!baslerInfo->camera->IsGrabbing()) // Check if camera is still grabbing
            break;

        _closeThreadMtx.lock();
        if(_closeThread == true) // Check if a shutdown is happening
            break;
        _closeThreadMtx.unlock();

        ONLY_DEBUG(M_PRINT("Retrieving image"));
        baslerInfo->camera->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_Return); // Retrieve frame with a 5000 ms timeout

        ONLY_DEBUG(M_PRINT("Processing result"));
        if(ptrGrabResult->GrabSucceeded()) { // If a frame is grabbed...
            ONLY_DEBUG(M_PRINT("SizeX: %d", ptrGrabResult->GetWidth()));
            ONLY_DEBUG(M_PRINT("SizeY: %d", ptrGrabResult->GetHeight()));
            ONLY_DEBUG(M_PRINT("Buffer Size: %ld", ptrGrabResult->GetBufferSize()));

            // Downscale image
            uint8_t* newImageBuffer = (uint8_t*)ptrGrabResult->GetBuffer();
            Mat img(Size(static_cast<int>(width.GetValue()), static_cast<int>(height.GetValue())), CV_8UC3, newImageBuffer);
            resize(img, img, Size(RESX, RESY));
            ONLY_DEBUG(Size imgSize = img.size());
            ONLY_DEBUG(M_PRINT("New Dimensions X: %d", imgSize.width));
            ONLY_DEBUG(M_PRINT("New Dimensions Y: %d", imgSize.height));

            _mtx.lock();
            memcpy(_imageBuffer, img.data, RESX * RESY * BYTES_PER_PIXEL); // Save copy of frame
            _mtx.unlock();
        } else {
            M_ERR("Failed to grab image");
        }

        lock.Unlock();
    }

    _closeThreadMtx.unlock();
    lock.Unlock();
}

bool Recorder::StartRecording() {
    // Launch record thread
    M_PRINT("Launching recorder thread");

    _recordThread = new std::thread(RecordThread, (void *)&_threadInfo);
    if(_recordThread == nullptr)
        return false;

    return true;
}

// This method of copying frames eliminates a useless copy function
void Recorder::CopyFrameToQT(QImage *img) {
    ONLY_DEBUG(M_PRINT("Grabbing Frame"));
    int arrpos = 0;

    _mtx.lock();
    for(int y = 0; y < RESY; y++) { // Copy frame into buffer (This is very slow but works for now)
        for(int x = 0; x < RESX; x++) {
            img->setPixel(x, y, qRgb(0xFF & (int)_imageBuffer[arrpos], 0xFF & (int)_imageBuffer[arrpos+1], 0xFF & (int)_imageBuffer[arrpos+2]));
            arrpos += 3;
        }
    }
    _mtx.unlock();
}