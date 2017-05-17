#include "mapui.h"
#include "pathlayer.h"
#include "distanceindicator.h"
#include <algorithm>

namespace LIMOSIM
{

MapUi *mapUiInstance = 0;

MapUi::MapUi(QQuickItem *_parent) :
    UiElement(_parent),
    m_maxHeight(9000),
    m_maxWidth(9000)
{
    mapUiInstance = this;

    m_pathLayer = new PathLayer(this);

    connect(this, SIGNAL(xChanged()), this, SLOT(onMoved()));

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

MapUi* MapUi::getInstance()
{
    return mapUiInstance;
}

void MapUi::handleScale(double _factor, double _inc, double _x, double _y)
{
    double w = m_mapWidth;
    double h = m_mapHeight;

    setMapWidth(m_maxWidth * _factor / 5);
    setMapHeight(m_maxHeight * _factor / 5);

    updateVisibleArea();
    update();


    QPointF inc = QPointF(m_mapWidth-w, m_mapHeight-h)/2;
   // setOffset(p_viewPort->getCenter() - getVirtualCanvasPosition(QPointF(_x, _y))  + inc);


    QPointF map(m_mapWidth/2, m_mapHeight/2);
    //centerAt(map+inc);

   // setOffset(p_viewPort->getCenter() - map);
    setOffset(m_offset-inc);

}

void MapUi::centerInViewPort()
{
    QPointF map(m_mapWidth/2, m_mapHeight/2);
    centerAt(map);
}

void MapUi::centerAt(const QPointF &_position)
{
    setOffset(p_viewPort->getCenter() - _position);
}

void MapUi::zoomIn()
{
    p_settings->handleScale(p_settings->getScaleFactor() + 0.1, 0, 0);
}

void MapUi::zoomOut()
{
    p_settings->handleScale(p_settings->getScaleFactor() - 0.1, 0, 0);
}

QPointF MapUi::getOrigin()
{
    QPointF center(width()/2, height()/2);
    QPointF origin(0, 0);

    return origin;
}


QPointF MapUi::getViewPortPoint(const QPointF &_point)
{
    QPointF view(p_viewPort->width(), p_viewPort->height());

    QPointF point = _point;
    if(point.x()<0)
        point.setX(0);
    if(point.x()>view.x())
        point.setX(view.x());

    if(point.y()<0)
        point.setY(0);
    if(point.y()>view.y())
        point.setY(view.y());


    return point;
}

QPointF MapUi::getVirtualCanvasPosition(const QPointF &_point)
{
    double x = _point.x() - m_offset.x();
    double y = _point.y() - m_offset.y();

    return QPointF(x, y);
}

void MapUi::updateVisibleArea()
{
    QPointF origin(m_offset.x(), m_mapHeight + m_offset.y());
    QPointF end(m_mapWidth + m_offset.x(), m_offset.y());

    m_bl = getViewPortPoint(origin);
    m_tr = getViewPortPoint(end);

    m_bottomLeft = getMapPosition(m_bl);
    m_topRight = getMapPosition(m_tr);
}

bool MapUi::isVisible(const Position &_position)
{
    double x = _position.x;
    double y = _position.y;

    if(x>=m_bottomLeft.x && x<=m_topRight.x && y>=m_bottomLeft.y && y<=m_topRight.y)
        return true;
    return false;
}

double MapUi::getMapDistance(double _pixels)
{
    return _pixels / p_settings->getScaleFactor();
}

Position MapUi::getMapPosition(const QPointF &_point)
{
    QPointF offset = getOffset();
    double x = getMapDistance(_point.x() - offset.x());
    double y = getMapDistance(getMapHeight() + offset.y() - _point.y());

    return Position(x,y);
}




/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void MapUi::setMapWidth(double _width)
{
    m_mapWidth = _width;
   // setWidth(std::min(p_viewPort->width(), m_mapWidth));
}

void MapUi::setMapHeight(double _height)
{
    m_mapHeight = _height;
  //  setHeight(std::min(p_viewPort->height(), m_mapHeight));
}

void MapUi::setOffset(const QPointF &_offset)
{
    m_offset = _offset;

    updateVisibleArea();

    p_settings->repaint();
    update();
}

PathLayer* MapUi::getPathLayer()
{
    return m_pathLayer;
}

double MapUi::getMapWidth()
{
    return m_mapWidth;
}

double MapUi::getMapHeight()
{
    return m_mapHeight;
}

QPointF MapUi::getOffset()
{
    return m_offset;
}

/*************************************
 *              EPS EXPORT           *
 ************************************/



QString MapUi::toEps()
{
    QString data;

    data += m_eps.drawRect(this->boundingRect(), QPointF(0, 0), LineStyle("gray", 1, true, true));

    return data;
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void MapUi::onMoved()
{
    updateVisibleArea();
}

void MapUi::handlePaintEvent()
{
    p_painter->fillRect(this->boundingRect(), QColor("gray"));

    QPointF center(m_mapWidth/2 + m_offset.x(), m_mapHeight/2 + m_offset.y());
    LineStyle style("#1c#1c1c", 1, false, false, Qt::DashLine);
    drawLineOnCanvas(QPointF(center.x(), center.y()-m_mapHeight/2), QPointF(center.x(), center.y()+m_mapHeight/2), style);
    drawLineOnCanvas(QPointF(center.x()-m_mapWidth/2, center.y()), QPointF(center.x()+m_mapWidth/2, center.y()), style);
}





}
