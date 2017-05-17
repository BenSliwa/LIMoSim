#ifndef LIMOSIM_CARUI_H
#define LIMOSIM_CARUI_H


#include "mapelementui.h"
#include "core/vehicles/car.h"

namespace LIMOSIM
{


class CarUi : public MapElementUi
{
    Q_OBJECT
public:
    CarUi(QQuickItem *_parent = 0);

    void updateCarPosition();
    bool isVisible();

    // property accessors
    void setCar(Car *_car);
    void setColor(const QColor &_color);

    Car* getCar();
    QColor getColor();

    // EPS export
    QString toEps();

protected:
    void handlePaintEvent();
    void hoverEnterEvent(QHoverEvent *_event);
    void mousePressEvent(QMouseEvent *_event);



private:
    void drawId();
    void drawLeftTurnLight();
    void drawRightTurnLight();

private:
    Car *p_car;
    QColor m_color;

};


}


#endif // LIMOSIM_CARUI_H
