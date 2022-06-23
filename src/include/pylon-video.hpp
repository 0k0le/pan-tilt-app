/*
 * Matthew Todd Geiger
 * pylon-video.hpp
 */

#pragma once

// Pylon includes
#include <pylon/PylonIncludes.h>

// QT Includes
#include <QImage>

// STD Includes
#include <thread>
#include <mutex>

#define RESX 1600
#define RESY 900
#define BYTES_PER_PIXEL 3

#define MIN_EXPOSURE 12
#define MAX_EXPOSURE 999999

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

        void SetExposure(int percentage);
        bool StartRecording();
        void CopyFrameToQT(QImage *img);

    private:
        void InitializeCamera(const char* const cameraSerial);
        static void RecordThread(void *data); // Thread function must remain static
        DeviceInfoList_t::const_iterator FindCamera(const char* const cameraSerial, CTlFactory& TlFactory, DeviceInfoList_t& lstDevices);

        CMyInstantCamera *_camera = nullptr;
        RThreadData _threadInfo = {};
        
        std::thread *_recordThread = nullptr;
        static std::mutex _mtx;
        static std::mutex _closeThreadMtx;

        int _fps;
        int _qual;

        static bool _closeThread;
        static uint8_t _imageBuffer[RESX * RESY * BYTES_PER_PIXEL];
};