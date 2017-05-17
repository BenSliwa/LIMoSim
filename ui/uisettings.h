#ifndef UISETTINGS_H
#define UISETTINGS_H

#include <QObject>
#include <QDebug>
#include "core/settings/xmlparser.h"

#include "ui/plot/textstyle.h"
#include "ui/plot/linestyle.h"

namespace LIMOSIM
{

namespace ZOOM_LEVEL
{
    enum{
        PATH,
        SHAPE,
        DETAIL,
    };
}

class UiSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double scaleFactor READ getScaleFactor)
public:
    UiSettings(QObject *_parent = 0);
    static UiSettings* getInstance();

    bool hasValue(const QString &_group, const QString &_key);
    Variant getValue(const QString &_group, const QString &_key);
    DOMElement* getElement(const QString &_group, const QString &_key);

    TextStyle getTextStyle(DOMElement *_dom);
    LineStyle getLineStyle(DOMElement *_dom);


    Q_INVOKABLE void handleScale(double _factor, double _x, double _y);


    // property accessors
    double getScaleFactor();
    int getZoomLevel();


signals:
    void repaint();

public:
    double mapWidth;
    double mapHeight;

    QPointF offset;
    QMap<int,QString> nodeColors;
    QMap<int,QString> segmentColors;
    QMap<int,QString> wayColors;


    TextStyle labelStyle;
    TextStyle tickStyle;
    TextStyle legendHeaderStyle;
    TextStyle legendItemStyle;

    LineStyle segmentStyle;

    bool drawNodes;
    bool drawSegmentMarkers;



private:
    DOMElement *m_dom;


    double m_scaleFactor;

};

}


#endif // UISETTINGS_H
