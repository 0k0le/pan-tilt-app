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

    Recorder *recorder = new Recorder("40125042");

    recorder->StartRecording();

    QImage img(RESX, RESY, QImage::Format_RGB888);
    img.fill(QColor(Qt::red).rgb());

    QLabel l;
    l.setPixmap(QPixmap::fromImage(img));
    l.show();

    GUI gui(recorder, img, &l);
    gui.run();

    int ret = app.exec();

    delete recorder;

    return ret;
}