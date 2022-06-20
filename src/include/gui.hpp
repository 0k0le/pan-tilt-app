#pragma once

#include <QObject>
#include <QTimer>
#include <QImage>
#include <QLabel>
#include <QSlider>

#include "macros.hpp"
#include "pylon-video.hpp"

class GUI : public QObject
{
    Q_OBJECT

public:
    explicit GUI(QObject *parent = 0);
    virtual ~GUI();

    void run(); // call this when initialization is done

public slots:
    static void HandleHozSlider();

private:
    Recorder *recorder;
    QImage *img;
    QLabel *l;
    QSlider *hozSlider;
    QLabel *hozSliderLabel;
    QSlider *vertSlider;
    QLabel *vertSliderLabel;
};