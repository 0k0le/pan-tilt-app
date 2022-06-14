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

// Qt Includes
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtGui>
#include <QLabel>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Recorder *recorder = new Recorder("40125042");

    recorder->StartRecording();

    QImage img(RESX, RESY, QImage::Format_RGB888);
    img.fill(QColor(Qt::red).rgb());

    sleep(2);

    int arrpos = 0;
    uint8_t* buf = recorder->GetFrame();
    for(int y = 0; y < RESY; y++) {
        for(int x = 0; x < RESX; x++) {
            //M_PRINT("R(%d) G(%d) B(%d)", buf[arrpos], buf[arrpos+1], buf[arrpos+2]);
            img.setPixel(x, y, qRgb(0xFF & buf[arrpos], 0xFF & (int)buf[arrpos+1], 0xFF & (int)buf[arrpos+2]));
            arrpos += 3;
        }
    }

    QLabel l;
    l.setPixmap(QPixmap::fromImage(img));
    l.show();

    int ret = app.exec();

    delete recorder;

    return ret;
}