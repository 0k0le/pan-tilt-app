
#include "macros.hpp"
#include "moc/gui.moc"

GUI::GUI(Recorder *CRecorder, QImage &CImg, QLabel* label, QObject *parent) : QObject(parent) {
    recorder = CRecorder;
    img = CImg;
    l = label;
}

GUI::~GUI() {}

void GUI::run()
{
    auto timer = new QTimer();
    connect(timer, &QTimer::timeout, [this]{ 
        ONLY_DEBUG(M_PRINT("Grabbing Frame"));
        int arrpos = 0;

        uint8_t* buf = recorder->GetFrame();
        for(int y = 0; y < RESY; y++) {
            for(int x = 0; x < RESX; x++) {
                img.setPixel(x, y, qRgb(0xFF & buf[arrpos], 0xFF & (int)buf[arrpos+1], 0xFF & (int)buf[arrpos+2]));
                arrpos += 3;
            }
        }

        l->setPixmap(QPixmap::fromImage(img));
        l->show();
    });
    timer->start();
}