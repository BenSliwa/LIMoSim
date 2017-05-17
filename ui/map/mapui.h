#ifndef MAPUI_H
#define MAPUI_H

#include "ui/plot/uielement.h"
#include "core/location/position.h"

namespace LIMOSIM
{

class PathLayer;

class MapUi : public UiElement
{
    Q_OBJECT
public:
    MapUi(QQuickItem *_parent = 0);

    static MapUi* getInstance();

    void handleScale(double _factor, double _inc, double _x, double _y);
    void centerInViewPort();
    void centerAt(const QPointF &_position);

    void zoomIn();
    void zoomOut();
    QPointF getOrigin();
    QPointF getViewPortPoint(const QPointF &_point);
    QPointF getVirtualCanvasPosition(const QPointF &_point);
    void updateVisibleArea();
    bool isVisible(const Position &_position);


    double getMapDistance(double _pixels);
    Position getMapPosition(const QPointF &_point);


    // property accessors
    void setMapWidth(double _width);
    void setMapHeight(double _height);
    void setOffset(const QPointF &_offset);

    PathLayer* getPathLayer();
    double getMapWidth();
    double getMapHeight();
    QPointF getOffset();

    // EPS export
    QString toEps();




private slots:
    void onMoved();

private:
    void handlePaintEvent();


public:
    QPointF m_bl;
    QPointF m_tr;

private:
    PathLayer *m_pathLayer;
    double m_maxHeight;
    double m_maxWidth;
    double m_mapWidth;
    double m_mapHeight;
    QPointF m_offset;


    Position m_bottomLeft;
    Position m_topRight;








};

}

#endif // MAPUI_H
