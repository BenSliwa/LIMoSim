#ifndef LIMOSIM_PATHLAYER_H
#define LIMOSIM_PATHLAYER_H

#include "mapelementui.h"
#include "core/map/map.h"

namespace LIMoSim
{

class PathLayer : public MapElementUi
{
    Q_OBJECT
public:
    PathLayer(QQuickItem *_parent = 0);

    void setPath(const std::vector<Node*> &_path);
    void setHighlightedWay(Way *_way);

    void clear();

    // EPS export
    QString toEps();
    QString exportIntersections(int _zoomLevel);
    QString exportWay(Way *_way);
    QString exportSegment(Segment *_segment, int _zoomLevel);
    QString exportLanes(Segment *_segment);
    QString exportLane(Lane *_lane, const LineStyle &_style = LineStyle());
    QString exportConnectionLanes(Node *_node);

private:
    void handlePaintEvent();

    void drawIntersections(int _zoomLevel);
    void drawWays(int _zoomLevel);
    void drawWay(Way *_way, int _zoomLevel);
    void drawSegment(Segment *_segment, int _zoomLevel);
    void drawDirection(const Vector3d &_from, const Vector3d &_to);


    void mousePressEvent(QMouseEvent *_event);
    void mouseMoveEvent(QMouseEvent *_event);

    QList<Position> getIntersectionPath(Node *_node);

private slots:
    void onCarSelection(CarUi *_car);

signals:
    void mapPositionClicked(const Position &_position);

private:
    Map *p_map;
    std::vector<Node*> m_path;
    Way *p_highlightedWay;
    CarUi *p_highlightedCar;

    QPointF m_lastPosition;


};

}


#endif // LIMOSIM_PATHLAYER_H
