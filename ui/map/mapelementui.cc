#include "mapelementui.h"
#include "ui/uimanager.h"

namespace LIMOSIM
{

MapElementUi::MapElementUi(QQuickItem *_parent) :
    UiElement(_parent),
    p_mapUi(MapUi::getInstance()),
    p_eventManager(UiEventManager::getInstance()),
    p_uiManager(UiManager::getInstance()),
    p_settings(UiSettings::getInstance()),
    p_editor(RoadBuilder::getInstance()),
    m_selection(SELECTION::NO_SELECTION)
{
    connect(p_settings, SIGNAL(repaint()), this, SLOT(onRepaintRequest()));
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

double MapElementUi::getUiDistance(double _distance_m)
{
    return _distance_m * p_settings->getScaleFactor();
}

double MapElementUi::getMapDistance(double _pixels)
{
    return _pixels / p_settings->getScaleFactor();
}

QPointF MapElementUi::getUiPosition(const Position &_position)
{
    QPointF offset = p_mapUi->getOffset();
    double x = getUiDistance(_position.x) + offset.x();
    double y = p_mapUi->getMapHeight() + offset.y() - getUiDistance(_position.y);

    return QPointF(x,y);
}

Position MapElementUi::getMapPosition(const QPointF &_point)
{
    QPointF offset = p_mapUi->getOffset();
    double x = getMapDistance(_point.x() - offset.x());
    double y = getMapDistance(p_mapUi->getMapHeight() + offset.y() - _point.y());

    return Position(x,y);
}

void MapElementUi::centerAt(const QPointF &_position)
{
    QPointF position = _position;
    position.setX(position.x() - width()/2);
    position.setY(position.y() - height()/2);

    setPosition(position);
}

void MapElementUi::drawLine(const Position &_from, const Position &_to, const LineStyle &_style)
{
    QPointF from = getUiPosition(_from);
    QPointF to = getUiPosition(_to);

    p_painter->setPen(_style.toPen());
    p_painter->drawLine(from, to);
}

QPointF MapElementUi::getCanvasPoint(const Vector3d &_position)
{
    return QPointF(_position.x * p_settings->getScaleFactor(), _position.y * p_settings->getScaleFactor());
}


void MapElementUi::softSelection()
{
    if(m_selection==SELECTION::NO_SELECTION)
        setSelection(SELECTION::SOFT_SELECTION);
}

void MapElementUi::softDeselection()
{
    if(m_selection==SELECTION::SOFT_SELECTION)
        setSelection(SELECTION::NO_SELECTION);
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void MapElementUi::setSelection(int _type)
{
    m_selection = _type;

    update();
}

int MapElementUi::getSelection()
{
    return m_selection;
}


/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void MapElementUi::onRepaintRequest()
{
    update();
}

}
