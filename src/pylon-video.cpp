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
    if(_videoWriter != nullptr) 
        delete _videoWriter;

    if(_camera != nullptr)
        delete _camera;

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
        DeviceInfoList_t::const_iterator it;
        if(!lstDevices.empty()) {
            for(it = lstDevices.begin(); it != lstDevices.end(); ++it) {
                M_PRINT("Camera found on network: %s", it->GetSerialNumber().c_str());
                if(strcmp(it->GetSerialNumber().c_str(), cameraSerial) == 0) {
                    cameraFound = true;
                    break;
                }
            }
        }
        
        if(!cameraFound)
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
        CEnumParameter pixelFormat(_camera->GetNodeMap(), "PixelFormat");

        // Set camera properties
        width.TrySetValue(640, IntegerValueCorrection_Nearest);
        height.TrySetValue(480, IntegerValueCorrection_Nearest);

        CPixelTypeMapper pixelTypeMapper(&pixelFormat);
        EPixelType pixelType = pixelTypeMapper.GetPylonPixelTypeFromNodeValue(pixelFormat.GetIntValue());

        // Apply camera configuration
        _videoWriter->SetParameter(
            (uint32_t) width.GetValue(),
            (uint32_t) height.GetValue(),
            pixelType,
            _fps,
            _qual           
        );

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