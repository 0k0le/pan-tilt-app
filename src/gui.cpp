
#include "macros.hpp"
#include "pylon-video.hpp"
#include "moc/gui.moc"

GUI::GUI(QObject *parent) : QObject(parent) {
    recorder = new Recorder("40125042"); // Initialize recorder and establish connection with camera
    img = new QImage(RESX, RESY, QImage::Format_RGB888); // Create image buffer to load recorder frames
    l = new QLabel; // Create label to paste image on

    img->fill(QColor(Qt::red).rgb()); // Fill image with test color for debug

    l->setPixmap(QPixmap::fromImage(*img)); // Apply image to label
    l->setFixedSize(RESX + 240, RESY);
    l->show();

    // Setup labels
    hozSliderLabel = new QLabel(l);
    hozSliderLabel->setGeometry(RESX + 20, 10, 100, 20);
    hozSliderLabel->setText("Horizontal Axis");
    hozSliderLabel->show();

    // Setup sliders
    hozSlider = new QSlider(Qt::Horizontal, l);
    hozSlider->setGeometry(QRect(QPoint(RESX + 20, 40), QSize(200, 50)));
    hozSlider->setFocusPolicy(Qt::StrongFocus);
    hozSlider->setTickPosition(QSlider::TicksBothSides);
    hozSlider->setSingleStep(1);
    hozSlider->show();

    // Setup labels
    vertSliderLabel = new QLabel(l);
    vertSliderLabel->setGeometry(RESX + 20, 100, 100, 20);
    vertSliderLabel->setText("Vertical Axis");
    vertSliderLabel->show();

    // Setup sliders
    vertSlider = new QSlider(Qt::Horizontal, l);
    vertSlider->setGeometry(QRect(QPoint(RESX + 20, 130), QSize(200, 50)));
    vertSlider->setFocusPolicy(Qt::StrongFocus);
    vertSlider->setTickPosition(QSlider::TicksBothSides);
    vertSlider->setSingleStep(1);
    vertSlider->show();

    connect(hozSlider, &QSlider::sliderMoved, this, GUI::HandleHozSlider);

    // Start swapping image buffers and grabbing frames
    recorder->StartRecording();
}

GUI::~GUI() {
    delete recorder;
    delete img;
    delete l;
}

void GUI::HandleHozSlider() {
    M_PRINT("Moving debug slider");
}

void GUI::run()
{
    // Create timer for grabbing frames
    auto timer = new QTimer();
    connect(timer, &QTimer::timeout, [this]{ // lambda function to be called every timeout
        recorder->CopyFrameToQT(img);
        l->setPixmap(QPixmap::fromImage(*img));
    });
    timer->start();
}