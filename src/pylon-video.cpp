/*
 * Matthew Todd Geiger
 * pylon-video.cpp
 */

// Local includes
#include "macros.hpp"
#include "pylon-video.hpp"

using namespace Pylon;
using namespace GenApi;

Recorder::Recorder(const char* const cameraSerial, int framesPerSecond, int quality) {
    _fps = framesPerSecond;
    _qual = quality;

    InitializeCamera(cameraSerial); // Initialize the camera on startup
}

Recorder::~Recorder() {
    if(_camera != nullptr) {
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
    M_PRINT("Initializing Pylon/Basler camera API");
    PylonInitialize();
    
    CTlFactory& TlFactory = CTlFactory::GetInstance(); // User transport layer factory to communicate with basler devices over transport layer
    DeviceInfoList_t lstDevices;

    try {
        // Connect to camera
        M_PRINT("Locating camera: %s", cameraSerial);
        
        DeviceInfoList_t::const_iterator it = FindCamera(cameraSerial, TlFactory, lstDevices);

        if(it == nullptr)
            throw GENERIC_EXCEPTION("Failed to find camera with serial: %s", cameraSerial);

        M_PRINT("Connecting to: %s", cameraSerial);
        _camera = new CInstantCamera(TlFactory.CreateDevice(*it));
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
        width.TrySetValue(640, IntegerValueCorrection_Nearest);
        height.TrySetValue(480, IntegerValueCorrection_Nearest);
        offsetX.TrySetToMinimum();
        offsetY.TrySetToMinimum();
        pixelFormat.SetIntValue(PixelType_RGB8packed);

    } catch (const GenericException& e) {
        M_FATAL("An exception occured. %s", e.GetDescription());
    }

    // return
}

bool Recorder::StartRecording() {
    // Launch record thread
    M_PRINT("Launching recorder thread");

    // return
}

void Recorder::RecordThread(void *data) {
    RThreadData *baslerInfo = (RThreadData *)data;    

    //WHILE

    // Capture image

    // Write image to list
    
    //END WHILE

}