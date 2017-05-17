#include "viewport.h"
#include "distanceindicator.h"

namespace LIMOSIM
{

ViewPort *viewPortInstance = 0;

ViewPort::ViewPort(QQuickItem *_parent) :
    QQuickPaintedItem(_parent),
    m_distanceIndicator(new DistanceIndicator(this))
{
    viewPortInstance = this;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

ViewPort* ViewPort::getInstance()
{
    return viewPortInstance;
}

QPointF ViewPort::getCenter()
{
    return QPointF(width()/2, height()/2);
}


/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

DistanceIndicator* ViewPort::getDistanceIndicator()
{
    return m_distanceIndicator;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void ViewPort::paint(QPainter *_painter)
{
    _painter->fillRect(this->boundingRect(), "lightgray");

    m_distanceIndicator->updatePosition();
}

}
