/*
 * Matthew Todd Geiger
 * layout.hpp
 */

#pragma once

#include "macros.hpp"
#include "network.hpp"

#include <QObject>
#include <QSlider>
#include <QLabel>

typedef struct DispatchData {
    char msg[5];
    int data;
} DISPATCHDATA;

class Layout : public QObject {
    Q_OBJECT
    
    public:
        explicit Layout(Recorder* recorder, QLabel* parent, const char* const bbgIp);
        virtual ~Layout();

    private:
        // Rotation objects
        QSlider* xRotationSlider;
        QLabel* xRotationLabel;
        QSlider* yRotationSlider;
        QLabel* yRotationLabel;

        // Camera effects objects
        QSlider* focusSlider;
        QSlider* zoomSlider;
        QSlider* apertureSlider;
        QSlider* exposureSlider;
        QSlider* whitebalanceSlider;
        QSlider* gainSlider;
        
        QLabel* focusLabel;
        QLabel* zoomLabel;
        QLabel* apertureLabel;
        QLabel* exposureLabel;
        QLabel* whitebalanceLabel;
        QLabel* gainLabel;

        Client* client;
        Recorder* _recorder;

        std::thread *dispatchThread = nullptr;

        static void DispatchThread(void *data);
        static std::mutex dlock;
        static DISPATCHDATA dData;
};