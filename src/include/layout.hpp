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
        std::unique_ptr<QSlider> xRotationSlider;
        std::unique_ptr<QLabel> xRotationLabel;
        std::unique_ptr<QSlider> yRotationSlider;
        std::unique_ptr<QLabel> yRotationLabel;

        // Camera effects objects
        std::unique_ptr<QSlider> focusSlider;
        std::unique_ptr<QSlider> zoomSlider;
        std::unique_ptr<QSlider> apertureSlider;
        std::unique_ptr<QSlider> exposureSlider;
        std::unique_ptr<QSlider> whitebalanceSlider;
        std::unique_ptr<QSlider> gainSlider;
        
        std::unique_ptr<QLabel> focusLabel;
        std::unique_ptr<QLabel> zoomLabel;
        std::unique_ptr<QLabel> apertureLabel;
        std::unique_ptr<QLabel> exposureLabel;
        std::unique_ptr<QLabel> whitebalanceLabel;
        std::unique_ptr<QLabel> gainLabel;

        std::unique_ptr<Client> client;
        
        Recorder* _recorder;
        std::thread *dispatchThread = nullptr;

        static void DispatchThread(void *data);
        static std::mutex dlock;
        static DISPATCHDATA dData;
};