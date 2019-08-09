#include "uisettings.h"
#include <QFile>
#include "ui/map/mapelementui.h"
#include "LIMoSim/map/mapdefinitions.h"
#include "ui/map/mapui.h"

namespace LIMoSim
{

UiSettings *uiSettingsInstance = nullptr;

UiSettings::UiSettings(QObject *_parent) :
    QObject(_parent),
    mapWidth(2000),
    mapHeight(2000),
    m_scaleFactor(5),
    drawNodes(true),
    drawSegmentMarkers(true)
{
    QFile file(QLatin1String(":/UiSettings.xml"));
    if(file.open(QIODevice::ReadOnly))
    {
        QString data = file.readAll();

        XMLParser parser;
        m_dom = parser.parseData(data.toStdString());


        nodeColors.insert(SELECTION::NO_SELECTION, QString::fromStdString(getValue("Node", "NoSelection").toString()));
        nodeColors.insert(SELECTION::SOFT_SELECTION, QString::fromStdString(getValue("Node", "SoftSelection").toString()));
        nodeColors.insert(SELECTION::HARD_SELECTION, QString::fromStdString(getValue("Node", "HardSelection").toString()));

        segmentColors.insert(SELECTION::NO_SELECTION, QString::fromStdString(getValue("Segment", "NoSelection").toString()));
        segmentColors.insert(SELECTION::SOFT_SELECTION, QString::fromStdString(getValue("Segment", "SoftSelection").toString()));
        segmentColors.insert(SELECTION::HARD_SELECTION, QString::fromStdString(getValue("Segment", "HardSelection").toString()));

        wayColors.insert(WAY_DIRECTION::FORWARD, QString::fromStdString(getValue("Way", "Forward").toString()));
        wayColors.insert(WAY_DIRECTION::BACKWARD, QString::fromStdString(getValue("Way", "Backward").toString()));
        wayColors.insert(WAY_DIRECTION::BIDIRECTIONAL, QString::fromStdString(getValue("Way", "Bidirectional").toString()));


        labelStyle = getTextStyle(getElement("Figure", "Label"));
        tickStyle = getTextStyle(getElement("Figure", "Tick"));
        legendHeaderStyle = getTextStyle(getElement("Figure", "LegendHeader"));
        legendItemStyle = getTextStyle(getElement("Figure", "LegendItem"));

        segmentStyle = getLineStyle(getElement("Map", "Segment"));
    }

}

UiSettings* UiSettings::getInstance()
{
    if(!uiSettingsInstance)
        uiSettingsInstance = new UiSettings();

    return uiSettingsInstance;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


bool UiSettings::hasValue(const QString &_group, const QString &_key)
{
    DOMElement *group = m_dom->getFirstChildWithTag(_group.toStdString());
    if(group)
    {
        DOMElement *node = group->getFirstChildWithTag(_key.toStdString());
        if(node)
            return true;
    }

    return false;
}

Variant UiSettings::getValue(const QString &_group, const QString &_key)
{
    Variant value;

    DOMElement *element = getElement(_group, _key);
    if(element)
        value = Variant(element->text);

    return value;
}

DOMElement* UiSettings::getElement(const QString &_group, const QString &_key)
{
    DOMElement *group = m_dom->getFirstChildWithTag(_group.toStdString());
    if(group)
    {
        DOMElement *node = group->getFirstChildWithTag(_key.toStdString());
        return node;
    }

    return nullptr;
}

TextStyle UiSettings::getTextStyle(DOMElement *_dom)
{
    TextStyle style;
    if(_dom)
    {
        style.font  = QString::fromStdString(_dom->getAttribute("font").toString());
        style.fontSize = _dom->getAttribute("size").toInt();
        style.color = QString::fromStdString(_dom->getAttribute("color").toString());

        if(_dom->hasAttribute("h"))
        {
            QString align = QString::fromStdString(_dom->getAttribute("h").toString());
            if(align=="left")
                style.horizontalAlignment = Qt::AlignLeft;
            else if(align=="right")
                style.horizontalAlignment = Qt::AlignRight;
            else
                style.horizontalAlignment = Qt::AlignCenter;
        }
    }

    return style;
}

LineStyle UiSettings::getLineStyle(DOMElement *_dom)
{
    LineStyle style;
    if(_dom)
    {
        style.color = QString::fromStdString(_dom->getAttribute("color").toString());
    }

    return style;
}

void UiSettings::handleScale(double _factor, double _x, double _y)
{
    MapUi *map = MapUi::getInstance();
    QPointF mouse(_x, _y);
    Position position = map->getMapPosition(mouse);

    if(_factor>=0.1)
    {
        double inc = _factor - m_scaleFactor;
        m_scaleFactor = _factor;

        if(m_scaleFactor<2.5)
        {
            drawNodes = false;
            drawSegmentMarkers = false;
        }
        else
        {
            drawNodes = true;
            drawSegmentMarkers = true;
        }


        map->handleScale(_factor, inc, _x, _y);

        emit repaint();
    }

    if(_x>0 && _y>0)
    {
        // zoom at the mouse position
        QPointF ui = map->getUiPosition(position);
        map->setOffset(map->getOffset() - (ui - mouse));
    }

}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

double UiSettings::getScaleFactor()
{
    return m_scaleFactor;
}

int UiSettings::getZoomLevel()
{
    int level = ZOOM_LEVEL::SHAPE;

    if(m_scaleFactor>2.5)
        level = ZOOM_LEVEL::DETAIL;
    else if(m_scaleFactor<0.3)
        level = ZOOM_LEVEL::PATH;

    return level;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

}
