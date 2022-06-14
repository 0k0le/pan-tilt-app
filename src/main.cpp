/*
 * Matthew Todd Geiger
 * main.cpp
 */

// Standard includes
#include <cstdlib>

// local includes
#include <macros.hpp>
#include <pylon-video.hpp>

// Qt Includes
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

int main(int argc, char** argv) {
    Recorder *recorder = new Recorder("40125042");

    recorder->StartRecording();

    delete recorder;
    return 0;
    //QApplication app(argc, argv);
    
    // Event loop
    //return app.exec();
}