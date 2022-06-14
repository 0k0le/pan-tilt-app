/*
 * Matthew Todd Geiger
 * pylon-video.cpp
 */

// Local includes
#include "macros.hpp"
#include "pylon-video.hpp"

using namespace Pylon;
using namespace GenApi;

Recorder::Recorder(const char* const cameraSerial) {
    InitializeCamera(cameraSerial); // Initialize the camera on startup
}

Recorder::~Recorder() {
    if(_videoWriter != nullptr) 
        delete _videoWriter;

    PylonTerminate();
}

void Recorder::InitializeCamera(const char* const cameraSerial) {
    // Initialize Pylon API
    M_PRINT("Initializing Pylon/Basler camera API");
    PylonInitialize();

    try {
        // Check for video support
        M_PRINT("Checking for required camera features");
        if(!CVideoWriter::IsSupported())
            throw GENERIC_EXCEPTION("VideoWriter is not supported, have you installed the MPEG-4 supplementary package from Basler website?");

        _videoWriter = new CVideoWriter;

        // Connect to camera
        M_PRINT("Connecting to camera: %s", cameraSerial);

        bool cameraFound = false;

        CTlFactory& TlFactory = CTlFactory::GetInstance();
        DeviceInfoList_t lstDevices;
        TlFactory.EnumerateDevices(lstDevices);
        if(!lstDevices.empty()) {
            DeviceInfoList_t::const_iterator it;
            for(it = lstDevices.begin(); it != lstDevices.end(); ++it) {
                M_PRINT("Camera Found: %s", it->GetSerialNumber().c_str());
                cameraFound = true;
            }
        }
        
        if(!cameraFound)
            throw GENERIC_EXCEPTION("Failed to find camera with serial: %s", cameraSerial);

        // Configure camera
        M_PRINT("Configuring camera settings");

    } catch (const GenericException& e) {
        M_FATAL("An exception occured. %s", e.GetDescription());
    }


    // return
}

bool StartRecording() {
    // Launch record thread
    M_PRINT("Launching recorder thread");

    // return
}

void RecordThread(void *data) {
    
    //WHILE

    // Capture image

    // Write image to list
    
    //END WHILE

}