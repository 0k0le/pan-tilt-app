
#include "macros.hpp"
#include "pylon-video.hpp"
#include "moc/gui.moc"

GUI::GUI(QObject *parent) : QObject(parent) {
    recorder = new Recorder("40125042"); // Initialize recorder and establish connection with camera
    img = new QImage(RESX, RESY, QImage::Format_RGB888); // Create image buffer to load recorder frames
    l = new QLabel; // Create label to paste image on

    img->fill(QColor(Qt::red).rgb()); // Fill image with test color for debug

    l->setPixmap(QPixmap::fromImage(*img)); // Apply image to label
    l->show();

    // Start swapping image buffers and grabbing frames
    recorder->StartRecording();
}

GUI::~GUI() {
    delete recorder;
    delete img;
    delete l;
}

void GUI::run()
{
    // Create timer for grabbing frames
    auto timer = new QTimer();
    connect(timer, &QTimer::timeout, [this]{ // lambda function to be called every timeout
        ONLY_DEBUG(M_PRINT("Grabbing Frame"));
        int arrpos = 0;

        uint8_t* buf = recorder->GetFrame(); // Grab most current frame from recorder
        for(int y = 0; y < RESY; y++) { // Copy frame into buffer (This is very slow but works for now)
            for(int x = 0; x < RESX; x++) {
                img->setPixel(x, y, qRgb(0xFF & (int)buf[arrpos], 0xFF & (int)buf[arrpos+1], 0xFF & (int)buf[arrpos+2]));
                arrpos += 3;
            }
        }

        l->setPixmap(QPixmap::fromImage(*img));
        //l->show();
    });
    timer->start();
}