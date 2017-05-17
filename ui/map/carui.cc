#include "carui.h"
#include "ui/map/pathlayer.h"
#include "core/map/osm/vehicleentry.h"

namespace LIMOSIM
{

CarUi::CarUi(QQuickItem *_parent) :
    MapElementUi(_parent),
    p_car(0),
    m_color("yellow")
{
    setZ(100);

    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);



}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void CarUi::updateCarPosition()
{
    double w  = getUiDistance(p_car->getLength());
    double h = getUiDistance(p_car->getWidth());

    if(w>=1 && h>=1)
    {
        setWidth(w);
        setHeight(h);
    }


    centerAt(getUiPosition(p_car->getCenter()));
    setRotation(-p_car->getOrientation().getYaw());
}

bool CarUi::isVisible()
{
    bool visible = p_mapUi->isVisible(p_car->getPosition());

    return visible;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void CarUi::setCar(Car *_car)
{
    p_car = _car;

    updateCarPosition();

    //if(p_car->getId()==0)
    //    SelectionHandler::getInstance()->selectCar(this);

}

void CarUi::setColor(const QColor &_color)
{
    m_color = _color;
}

Car* CarUi::getCar()
{
    return p_car;
}

QColor CarUi::getColor()
{
    return m_color;
}

QString CarUi::toEps()
{
    QString data;

    // increate the car size for the export
    double s = 1.5;
    double width = p_car->getWidth();
    p_car->setWidth(width*s);

    double length = p_car->getLength();
    p_car->setLength(length*s);

    BoundingBox box = p_car->getBoundingBox();

    QPolygonF polygon;
    polygon << getCanvasPoint(box.frontLeft);
    polygon <<getCanvasPoint(box.frontRight);
    polygon << getCanvasPoint(box.backRight);
    polygon << getCanvasPoint(box.backLeft);
    data += m_eps.drawPolygon(polygon, LineStyle(m_color.name(), 1, true, true));

    // restore the real car size
    p_car->setWidth(width);
    p_car->setLength(length);

    return data;
}

/*************************************
 *          ABSTRACT METHODS         *
 ************************************/

void CarUi::handlePaintEvent()
{
    if(isVisible())
    {
        int zoomLevel = p_settings->getZoomLevel();
        updateCarPosition();

        p_painter->fillRect(this->boundingRect(), m_color);


        if(zoomLevel==ZOOM_LEVEL::DETAIL)
            drawId();


        // TODO: here we need a layer which is ONLY displaying path information -> separate street and path layers
        p_mapUi->getPathLayer()->update();


        drawLeftTurnLight();
        drawRightTurnLight();
    }
}

void CarUi::hoverEnterEvent(QHoverEvent *_event)
{
    qDebug() << QString::fromStdString(p_car->getName()) << QString::fromStdString(p_car->getPosition().toString());
}

void CarUi::drawId()
{
    int s = 12;

    QFont font = p_painter->font() ;
    font.setPointSize(s-2);
    p_painter->setPen(QColor("black"));
    p_painter->setFont(font);
    p_painter->drawText(boundingRect(), Qt::AlignCenter, QString::fromStdString(p_car->getName()));
}

void CarUi::mousePressEvent(QMouseEvent *_event)
{
    if(_event->button()==Qt::LeftButton)
    {
        SelectionHandler::getInstance()->selectCar(this);
        //p_eventManager->handleCarSelection(this);

        VehicleEntry *entry = VehicleEntry::fromCar(p_car);
        std::cout << entry->toString(0) << std::endl;
        delete entry;

    }
}

void CarUi::drawLeftTurnLight()
{
    double s = 3;
    QPointF r = boundingRect().topRight();
    QRectF rect;
    rect.setTopRight(r);
    rect.setBottomLeft(QPointF(r.x()-3, r.y()+3));
    p_painter->fillRect(rect, "yellow");
}

void CarUi::drawRightTurnLight()
{
    double s = 3;
    QPointF r = boundingRect().bottomRight();
    QRectF rect;
    rect.setBottomRight(r);
    rect.setTopLeft(QPointF(r.x()-3, r.y()-3));
    p_painter->fillRect(rect, "yellow");
}


}
