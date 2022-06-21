/*
 * Matthew Todd Geiger
 * main.cpp
 */

// Standard includes
#include <cstdlib>
#include <unistd.h>

// local includes
#include <macros.hpp>
#include <pylon-video.hpp>

#include "gui.hpp"
#include "network.hpp"

// Qt Includes
#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QThread>

int main(int argc, char** argv) {
    if(argc < 3)
        M_FATAL("%s <CAMERA_SERIAL> <BBG_IP>", argv[0]);

    const char* const cameraSerial = argv[1];
    const char* const bbgIp = argv[2];

    Client* client = new Client(bbgIp);

    QApplication app(argc, argv);

    // QT object that controls the GUI
    GUI *gui = new GUI(cameraSerial);
    gui->run();

    // Execute
    int ret = app.exec();

    // Cleanup
    delete gui;
    delete client;

    return ret;
}