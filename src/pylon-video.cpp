/*
 * Matthew Todd Geiger
 * pylon-video.cpp
 */

// Local includes
#include "macros.hpp"
#include "pylon-video.hpp"

using namespace Pylon;
using namespace GenApi;

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
        
        _closeThreadMtx.lock();
        _closeThread = true;
        _closeThreadMtx.unlock();

        _recordThread->join();
        delete _recordThread;
    }

    if(_camera != nullptr) {
        M_PRINT("Closing camera handle");
        _camera->Close();
        delete _camera;
    }

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
        
        DeviceInfoList_t::const_iterator it = FindCamera(cameraSerial, TlFactory, lstDevices);

        if(it == nullptr)
            throw GENERIC_EXCEPTION("Failed to find camera with serial: %s", cameraSerial);

        M_PRINT("Connecting to: %s", cameraSerial);
        _camera = new CMyInstantCamera(TlFactory.CreateDevice(*it));
        M_PRINT("Camera connection successful");

        _camera->Open();

        // Configure camera
        M_PRINT("Configuring camera settings");
        
        // Get handle on camera properties
        CIntegerParameter width(_camera->GetNodeMap(), "Width");
        CIntegerParameter height(_camera->GetNodeMap(), "Height");
        CIntegerParameter offsetX(_camera->GetNodeMap(), "OffsetX");
        CIntegerParameter offsetY(_camera->GetNodeMap(), "OffsetY");
        CEnumParameter pixelFormat(_camera->GetNodeMap(), "PixelFormat");

        // Set camera properties
        width.TrySetValue(RESX, IntegerValueCorrection_Nearest);
        height.TrySetValue(RESY, IntegerValueCorrection_Nearest);
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

    CLock& lock = baslerInfo->camera->RetLock();

    M_PRINT("Initiating grabber");
    lock.Lock();
    baslerInfo->camera->StartGrabbing();
    lock.Unlock();

    CGrabResultPtr ptrGrabResult;

    while(1) {
        lock.Lock();
        if(!baslerInfo->camera->IsGrabbing())
            break;

        _closeThreadMtx.lock();
        if(_closeThread == true)
            break;
        _closeThreadMtx.unlock();

        ONLY_DEBUG(M_PRINT("Retrieving image"));
        baslerInfo->camera->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_Return);

        ONLY_DEBUG(M_PRINT("Processing result"));
        if(ptrGrabResult->GrabSucceeded()) {
            ONLY_DEBUG(M_PRINT("SizeX: %d", ptrGrabResult->GetWidth()));
            ONLY_DEBUG(M_PRINT("SizeY: %d", ptrGrabResult->GetHeight()));
            ONLY_DEBUG(M_PRINT("Buffer Size: %ld", ptrGrabResult->GetBufferSize()));

            _mtx.lock();
            memcpy(_imageBuffer, ptrGrabResult->GetBuffer(), RESX * RESY * BYTES_PER_PIXEL);
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

uint8_t* Recorder::GetFrame() {
    static uint8_t imageBuffer[RESX * RESY * BYTES_PER_PIXEL];

    _mtx.lock();
    memcpy(imageBuffer, _imageBuffer, RESX * RESY * BYTES_PER_PIXEL);
    _mtx.unlock();

    return imageBuffer;
}