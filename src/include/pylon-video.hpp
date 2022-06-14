/*
 * Matthew Todd Geiger
 * pylon-video.hpp
 */

#pragma once

// Pylon includes
#include <pylon/PylonIncludes.h>

using namespace Pylon;
using namespace GenApi;

typedef struct RecorderThreadData {
    CInstantCamera *camera;
} RThreadData;

class Recorder {
    public:
        Recorder(const char* const cameraSerial, int framesPerSecond = 30, int quality = 90);
        ~Recorder();

        bool StartRecording();

    private:
        void InitializeCamera(const char* const cameraSerial);
        static void RecordThread(void *data); // Thread function must remain static
        DeviceInfoList_t::const_iterator FindCamera(const char* const cameraSerial, CTlFactory& TlFactory, DeviceInfoList_t& lstDevices);

        CInstantCamera *_camera = nullptr;

        int _fps;
        int _qual;
};