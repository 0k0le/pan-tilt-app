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
    explicit GUI(QObject *parent = 0);
    virtual ~GUI();

    void run(); // call this when initialization is done

private:
    Recorder *recorder;
    QImage *img;
    QLabel *l;
};