
#include "macros.hpp"
#include "pylon-video.hpp"
#include "moc/gui.moc"

GUI::GUI(const char* const cameraSerial, const char* const bbgIp, QObject *parent) : QObject(parent) {
    recorder = new Recorder(cameraSerial); // Initialize recorder and establish connection with camera
    img = new QImage(RESX, RESY, QImage::Format_RGB888); // Create image buffer to load recorder frames
    l = new QLabel; // Create label to paste image on

    img->fill(QColor(Qt::red).rgb()); // Fill image with test color for debug

    l->setPixmap(QPixmap::fromImage(*img)); // Apply image to label
    l->setFixedSize(RESX + 290, RESY);
    l->show();

    layout = new Layout(recorder, l, bbgIp);

    // Start swapping image buffers and grabbing frames
    recorder->StartRecording();
}

GUI::~GUI() {
    delete recorder;
    delete l;
    //delete img;
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