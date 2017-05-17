#ifndef LIMOSIM_ROADBUILDER_H
#define LIMOSIM_ROADBUILDER_H

#include <QObject>
#include <QDebug>

#include "core/location/position.h"

namespace LIMOSIM
{

class Map;
class Node;
class NodeUi;
class PathLayer;
class Segment;
class SegmentMarker;
class UiManager;
class Way;

class RoadBuilder : public QObject
{
    Q_OBJECT
public:
    RoadBuilder(QObject *_parent = 0);
    static RoadBuilder* getInstance();

    void clear();
    Q_INVOKABLE void handleKey(int _key);

    void registerNode(NodeUi *_node);
    void selectNode(NodeUi *_node);
    void deleteNode(NodeUi *_node);


    void registerSegment(SegmentMarker *_segment);
    void selectSegment(SegmentMarker *_segment);
    void deleteSegment(SegmentMarker *_segment);

    void deselectAll();



    void registerPathLayer(PathLayer *_pathLayer);

private:
    NodeUi *createNode(const Position &_position, bool _link);
    void createSegmentUi(Segment *_segment);
    void createWayWithNode();




private slots:
    void onMapPositionClicked(const Position &_position);

    void onNodeLeftClicked();
    void onNodeRightClicked();

    void onSegmentLeftClicked();

private:
    Map *p_map;
    UiManager *p_uiManager;
    PathLayer *p_pathLayer;

    NodeUi *p_selectedNode;
    SegmentMarker *p_selectedSegment;
    Way *p_currentWay;
};

}

#endif // LIMOSIM_ROADBUILDER_H
