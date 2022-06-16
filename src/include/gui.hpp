#pragma once

#include <QObject>
#include <QTimer>
#include <QImage>
#include <QLabel>

#include "macros.hpp"
#include "pylon-video.hpp"

class GUI : public QObject
{
    Q_OBJECT
public:
    explicit GUI(Recorder *CRecorder, QImage &CImg, QLabel *label, QObject *parent = 0);
    virtual ~GUI();

    Recorder *recorder;
    QImage img;
    QLabel* l;

    void run(); // <-- you missed this
};