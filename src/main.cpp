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

// Qt Includes
#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QThread>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // QT object that controls the GUI
    GUI *gui = new GUI;
    gui->run();

    // Execute
    int ret = app.exec();

    // Cleanup
    delete gui;

    return ret;
}