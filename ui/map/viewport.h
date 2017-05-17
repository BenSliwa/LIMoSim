#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QPolygonF>
#include <QDebug>



namespace LIMoSim
{



class DistanceIndicator;

class ViewPort : public QQuickPaintedItem
{
    Q_OBJECT
public:
    ViewPort(QQuickItem *_parent = 0);

    static ViewPort* getInstance();

    QPointF getCenter();

    // property accessors
    DistanceIndicator* getDistanceIndicator();


protected:
    void paint(QPainter *_painter);

private:
    DistanceIndicator *m_distanceIndicator;

};


}

#endif // VIEWPORT_H
