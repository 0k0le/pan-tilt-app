/*
 * Matthew Todd Geiger
 * pylon-video.hpp
 */

#pragma once

// Pylon includes
#include <pylon/PylonIncludes.h>

#include <thread>
#include <mutex>

#define RESX 640
#define RESY 480
#define BYTES_PER_PIXEL 3

using namespace Pylon;
using namespace GenApi;

class CMyInstantCamera : public CInstantCamera {
    public:
        CMyInstantCamera(IPylonDevice *dev) : CInstantCamera(dev) {}

        CLock& RetLock() {
            static CLock& lock = CInstantCamera::GetLock();
            return lock;
        }
};

typedef struct RecorderThreadData {
    CMyInstantCamera *camera;
} RThreadData;

class Recorder {
    public:
        Recorder(const char* const cameraSerial, int framesPerSecond = 30, int quality = 90);
        ~Recorder();

        bool StartRecording();
        uint8_t* GetFrame();

    private:
        void InitializeCamera(const char* const cameraSerial);
        static void RecordThread(void *data); // Thread function must remain static
        DeviceInfoList_t::const_iterator FindCamera(const char* const cameraSerial, CTlFactory& TlFactory, DeviceInfoList_t& lstDevices);

        CMyInstantCamera *_camera = nullptr;
        RThreadData _threadInfo = {};
        
        std::thread *_recordThread = nullptr;
        static std::mutex _mtx;

        int _fps;
        int _qual;

        static uint8_t _imageBuffer[RESX * RESY * BYTES_PER_PIXEL];
};