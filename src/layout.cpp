/*
 * Matthew Todd Geiger
 * layout.cpp
 */

#include "pylon-video.hpp"
#include "moc/layout.moc"

Layout::Layout(QLabel* parent, const char* const bbgIp) : QObject(parent) {
    client = new Client(bbgIp);

    // Setup labels
    xRotationLabel = new QLabel(parent);
    xRotationLabel->setGeometry(RESX + 20, 10, 100, 20);
    xRotationLabel->setText("Horizontal Axis");
    xRotationLabel->show();

    // Setup sliders
    xRotationSlider = new QSlider(Qt::Horizontal, parent);
    xRotationSlider->setGeometry(QRect(QPoint(RESX + 20, 40), QSize(250, 50)));
    xRotationSlider->setMaximum(100);
    xRotationSlider->setMinimum(0);
    xRotationSlider->setFocusPolicy(Qt::StrongFocus);
    xRotationSlider->setTickPosition(QSlider::TicksBothSides);
    xRotationSlider->setSingleStep(1);
    xRotationSlider->show();

    // Setup labels
    yRotationLabel = new QLabel(parent);
    yRotationLabel->setGeometry(RESX + 20, 100, 100, 20);
    yRotationLabel->setText("Vertical Axis");
    yRotationLabel->show();

    // Setup sliders
    yRotationSlider = new QSlider(Qt::Horizontal, parent);
    yRotationSlider->setGeometry(QRect(QPoint(RESX + 20, 130), QSize(250, 50)));
    yRotationSlider->setFocusPolicy(Qt::StrongFocus);
    yRotationSlider->setTickPosition(QSlider::TicksBothSides);
    yRotationSlider->setSingleStep(1);
    yRotationSlider->show();

    // Vertical Sliders
    focusSlider = new QSlider(Qt::Vertical, parent);
    focusSlider->setGeometry(RESX + 27, 230, 50, 200);
    focusSlider->setFocusPolicy(Qt::StrongFocus);
    focusSlider->setTickPosition(QSlider::TicksBothSides);
    focusSlider->setSingleStep(1);
    focusSlider->show();

    // Vertical Sliders
    zoomSlider = new QSlider(Qt::Vertical, parent);
    zoomSlider->setGeometry(RESX + 123, 230, 50, 200);
    zoomSlider->setFocusPolicy(Qt::StrongFocus);
    zoomSlider->setTickPosition(QSlider::TicksBothSides);
    zoomSlider->setSingleStep(1);
    zoomSlider->show();

    // Vertical Sliders
    apertureSlider = new QSlider(Qt::Vertical, parent);
    apertureSlider->setGeometry(RESX + 218, 230, 50, 200);
    apertureSlider->setFocusPolicy(Qt::StrongFocus);
    apertureSlider->setTickPosition(QSlider::TicksBothSides);
    apertureSlider->setSingleStep(1);
    apertureSlider->show();

    // Vertical Sliders
    exposureSlider = new QSlider(Qt::Vertical, parent);
    exposureSlider->setGeometry(RESX + 27, 480, 50, 200);
    exposureSlider->setFocusPolicy(Qt::StrongFocus);
    exposureSlider->setTickPosition(QSlider::TicksBothSides);
    exposureSlider->setSingleStep(1);
    exposureSlider->show();

    // Vertical Sliders
    whitebalanceSlider = new QSlider(Qt::Vertical, parent);
    whitebalanceSlider->setGeometry(RESX + 123, 480, 50, 200);
    whitebalanceSlider->setFocusPolicy(Qt::StrongFocus);
    whitebalanceSlider->setTickPosition(QSlider::TicksBothSides);
    whitebalanceSlider->setSingleStep(1);
    whitebalanceSlider->show();

    // Vertical Sliders
    gainSlider = new QSlider(Qt::Vertical, parent);
    gainSlider->setGeometry(RESX + 218, 480, 50, 200);
    gainSlider->setFocusPolicy(Qt::StrongFocus);
    gainSlider->setTickPosition(QSlider::TicksBothSides);
    gainSlider->setSingleStep(1);
    gainSlider->show();

    // vertical labels
    focusLabel = new QLabel(parent);
    focusLabel->setGeometry(RESX + 30, 200, 100, 20);
    focusLabel->setText("Focus");
    focusLabel->show();

    // vertical labels
    zoomLabel = new QLabel(parent);
    zoomLabel->setGeometry(RESX + 126, 200, 100, 20);
    zoomLabel->setText("Zoom");
    zoomLabel->show();

    // vertical labels
    apertureLabel = new QLabel(parent);
    apertureLabel->setGeometry(RESX + 213, 200, 100, 20);
    apertureLabel->setText("Aperture");
    apertureLabel->show();

    // vertical labels
    exposureLabel = new QLabel(parent);
    exposureLabel->setGeometry(RESX + 20, 450, 100, 20);
    exposureLabel->setText("Exposure");
    exposureLabel->show();

    // vertical labels
    whitebalanceLabel = new QLabel(parent);
    whitebalanceLabel->setGeometry(RESX + 103, 450, 100, 20);
    whitebalanceLabel->setText("White Balance");
    whitebalanceLabel->show();

    // vertical labels
    gainLabel = new QLabel(parent);
    gainLabel->setGeometry(RESX + 225, 450, 100, 20);
    gainLabel->setText("Gain");
    gainLabel->show();

    connect(focusSlider, &QSlider::valueChanged, this, [this]{
        this->client->RequestChange(FOCUS_CONTROL, 50);
    });
}

Layout::~Layout() {
    M_PRINT("Destroying layout");
    delete xRotationLabel;
    delete yRotationLabel;
    delete xRotationSlider;
    delete yRotationSlider;

    delete focusSlider;
    delete zoomSlider;
    delete apertureSlider;
    delete exposureSlider;
    delete whitebalanceSlider;
    delete gainSlider;

    delete focusLabel;
    delete zoomLabel;
    delete apertureLabel;
    delete exposureLabel;
    delete whitebalanceLabel;
    delete gainLabel;

    delete client;
}