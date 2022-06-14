/*
 * Matthew Todd Geiger
 * pylon-video.hpp
 */

#pragma once

// Pylon includes
#include <pylon/PylonIncludes.h>

class Recorder {
    public:
        Recorder(const char* const cameraSerial);
        ~Recorder();

    private:
        void InitializeCamera(const char* const cameraSerial);

        Pylon::CVideoWriter *_videoWriter = nullptr;
};