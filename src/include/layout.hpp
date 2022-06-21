/*
 * Matthew Todd Geiger
 * layout.hpp
 */

#include "macros.hpp"

#include <QObject>
#include <QSlider>
#include <QLabel>

class Layout : public QObject {
    Q_OBJECT
    
    public:
        explicit Layout(QLabel* parent);
        virtual ~Layout();
    
    private:
        QSlider *hozSlider;
        QLabel *hozSliderLabel;
        QSlider *vertSlider;
        QLabel *vertSliderLabel;
};