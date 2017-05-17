#include "distanceindicator.h"

namespace LIMoSim
{

DistanceIndicator::DistanceIndicator(QQuickItem *_parent) :
    MapElementUi(_parent)
{
    setWidth(200);
    setHeight(30);
    setZ(9000);


}

void DistanceIndicator::updatePosition()
{
    double x = p_parent->width() - width() - 50;
    double y = p_parent->height() - height() - 50;

    setPosition(QPointF(x,y));
}


void DistanceIndicator::handlePaintEvent()
{
    double w = width() - 1;
    double h = height() - 1;

    LineStyle style;
    drawLineOnCanvas(QPointF(0, h), QPointF(w, h), style);
    drawLineOnCanvas(QPointF(0, h), QPointF(0, h/2), style);
    drawLineOnCanvas(QPointF(w, h), QPointF(w, h/2), style);

    double distance_m = getMapDistance(w);
    double scaleFactor = p_settings->getScaleFactor();
    drawText(QString::number(distance_m) + " m / " + QString::number(scaleFactor), QPointF(w/2, h/4*3), TextStyle());

    p_mapUi = MapUi::getInstance();
    QPointF offset = p_mapUi->getOffset();
    QString text = QString::number(offset.x()) + " / " + QString::number(offset.y());


    QPointF center = (p_mapUi->m_tr - p_mapUi->m_bl)/2;
    center = p_mapUi->getVirtualCanvasPosition(center);
    QString t1 = QString::number(center.x()) + " / " + QString::number(center.y());

    drawText(text + " :: " + t1, QPointF(w/2, h/4*1), TextStyle("Arial", 10, "white"));

}


/*************************************
 *              EPS EXPORT           *
 ************************************/

QString DistanceIndicator::toEps()
{
    QString data;

    double w = width();
    double h = height();

    QPointF offset;
    offset.setX(50);
    offset.setY(50);

    LineStyle style;
    data += m_eps.drawLine(QPointF(0, 0) + offset, QPointF(w, 0) + offset);
    data += m_eps.drawLine(QPointF(0, 0) + offset, QPointF(0, h/2) + offset);
    data += m_eps.drawLine(QPointF(w, 0) + offset, QPointF(w, h/2) + offset);

    double distance_m = getMapDistance(w);
    data += m_eps.drawText(QString::number(distance_m) + " m", QPointF(w/2, h/4) + offset, TextStyle());


    return data;
}

}
