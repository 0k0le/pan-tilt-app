/*
 * Matthew Todd Geiger
 * layout.cpp
 */

#include "pylon-video.hpp"
#include "moc/layout.moc"

Layout::Layout(QLabel* parent) : QObject(parent) {
    
    // Setup labels
    hozSliderLabel = new QLabel(parent);
    hozSliderLabel->setGeometry(RESX + 20, 10, 100, 20);
    hozSliderLabel->setText("Horizontal Axis");
    hozSliderLabel->show();

    // Setup sliders
    hozSlider = new QSlider(Qt::Horizontal, parent);
    hozSlider->setGeometry(QRect(QPoint(RESX + 20, 40), QSize(200, 50)));
    hozSlider->setFocusPolicy(Qt::StrongFocus);
    hozSlider->setTickPosition(QSlider::TicksBothSides);
    hozSlider->setSingleStep(1);
    hozSlider->show();

    // Setup labels
    vertSliderLabel = new QLabel(parent);
    vertSliderLabel->setGeometry(RESX + 20, 100, 100, 20);
    vertSliderLabel->setText("Vertical Axis");
    vertSliderLabel->show();

    // Setup sliders
    vertSlider = new QSlider(Qt::Horizontal, parent);
    vertSlider->setGeometry(QRect(QPoint(RESX + 20, 130), QSize(200, 50)));
    vertSlider->setFocusPolicy(Qt::StrongFocus);
    vertSlider->setTickPosition(QSlider::TicksBothSides);
    vertSlider->setSingleStep(1);
    vertSlider->show();

    //connect(hozSlider, &QSlider::sliderMoved, this, Layout::HandleHozSlider);
}

Layout::~Layout() {

}