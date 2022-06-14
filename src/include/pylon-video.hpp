/*
 * Matthew Todd Geiger
 * pylon-video.hpp
 */

#pragma once

// Pylon includes
#include <pylon/PylonIncludes.h>

class Recorder {
    public:
        Recorder(const char* const cameraSerial, int framesPerSecond = 30, int quality = 90);
        ~Recorder();

    private:
        void InitializeCamera(const char* const cameraSerial);

        Pylon::CVideoWriter *_videoWriter = nullptr;
        Pylon::CInstantCamera *_camera = nullptr;

        int _fps;
        int _qual;
};