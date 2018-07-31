#ifndef LIMOSIM_SEGMENT_H
#define LIMOSIM_SEGMENT_H

#include "LIMoSim/location/position.h"
#include <sstream>
#include <vector>
#include <map>
#include <iostream>

#include "mapdefinitions.h"

namespace LIMoSim
{
class Lane;
class LaneGate;
class Node;
class Segment;
class Way;

struct SegmentGate
{
    Node *node;
    Position position;
    std::map<int,Segment*> segments;
    double stretchFactor;

    bool contains(int _turn)
    {
        return (segments.count(_turn)>0);
    }

    Segment* getNearestLeftSegment()
    {
        Segment *segment = 0;


        if(contains(TURN::SHARP_LEFT))
            segment = segments[TURN::SHARP_LEFT];
        else if(contains(TURN::LEFT))
            segment = segments[TURN::LEFT];
        else if(contains(TURN::SLIGHT_LEFT))
            segment = segments[TURN::SLIGHT_LEFT];
        else if(contains(TURN::THROUGH))
            segment = segments[TURN::THROUGH];
        else if(contains(TURN::SLIGHT_RIGHT))
            segment = segments[TURN::SLIGHT_RIGHT];
        else if(contains(TURN::RIGHT))
            segment = segments[TURN::RIGHT];
        else if(contains(TURN::SHARP_RIGHT))
            segment = segments[TURN::SHARP_RIGHT];

        return segment;
    }

    Segment* getNearestRightSegment()
    {
        Segment *segment = 0;



        if(contains(TURN::SHARP_RIGHT))
            segment = segments[TURN::SHARP_RIGHT];
        else if(contains(TURN::RIGHT))
            segment = segments[TURN::RIGHT];
        else if(contains(TURN::SLIGHT_RIGHT))
            segment = segments[TURN::SLIGHT_RIGHT];
        else if(contains(TURN::THROUGH))
            segment = segments[TURN::THROUGH];
        else if(contains(TURN::SLIGHT_LEFT))
            segment = segments[TURN::SLIGHT_LEFT];
        else if(contains(TURN::LEFT))
            segment = segments[TURN::LEFT];
        else if(contains(TURN::SHARP_LEFT))
            segment = segments[TURN::SHARP_LEFT];



        return segment;
    }


};

struct Rect
{
    Position startLeft;
    Position startRight;
    Position endLeft;
    Position endRight;

    double startStretch;
    double endStretch;

    void info()
    {
        std::cout << startLeft.toString() << "\t";
        std::cout << startRight.toString() << "\t";
        std::cout << endLeft.toString() << "\t";
        std::cout << endRight.toString() << "\t";
        std::cout << std::endl;
    }
};

class Segment
{
public:
    Segment(Node *_start, Node *_end, Way *_way);
    ~Segment();

    void clear();
    void linkLanes();
    void linkLanes(Lane *_out, LaneGate *_outGate, Lane *_in, LaneGate *_inGate, int _turnType);
    void clearLaneLinks(Node *_node);
    void connectTo(Segment *_segment, Node *_node, int _turnType, bool _direct=true);
    void updateConnectionLane(Lane *_from, Lane *_to, Node *_node, int _turnType);
    SegmentGate* getGateForNode(Node *_node);
    void updateGatePosition(SegmentGate *_gate);

    void updateLanePositions(int _index);

    void computeRawRect();
    void computeFinalRect();




    Position getCenter();
    Vector3d getDirection();
    double getLength();
    double getRotation();

    Node* getOtherNode(Node *_node);


    std::string toString();

    // lanes
    int getNumLanes() const { return m_lanes.size(); }
    int getLaneIndex(Lane *_lane);
    Lane* getLane(int _index);
    std::vector<Lane*> getBackwardLanes();
    std::vector<Lane*> getFowardLanes(); // incl. bidirectional


    // property accessors
    void setStart(Node *_start);
    void setEnd(Node *_end);
    void setPredecessor(Segment *_predecessor);
    void setSuccessor(Segment *_succsessor);

    SegmentGate* getStartGate();
    SegmentGate* getEndGate();
    Way* getWay();
    Segment* getPredecessor();
    Segment* getSuccessor();
    std::vector<Lane*>& getLanes();
    Rect& getRect();

private:
    Vector3d computeMeanDirection(Segment *_from, Segment *_to, bool _invertOut, bool _invertIn);
    double computeMeanRotation(Segment *_from, Segment *_to, bool _invertOut, bool _invertIn);
    double computeStretchFactor(double _angle0, double _angle1, bool _invertOut);

private:
    SegmentGate m_startGate;
    SegmentGate m_endGate;

    Way *p_way;

    Segment *p_predecessor;
    Segment *p_successor;

    std::vector<Lane*> m_lanes;

    Rect m_rect;
};

}

#endif // LIMOSIM_SEGMENT_H
