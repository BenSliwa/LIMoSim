#ifndef LIMOSIM_WAY_H
#define LIMOSIM_WAY_H

#include "mapobject.h"
#include <vector>
#include "node.h"
#include "segment.h"


namespace LIMOSIM
{

class Way : public MapObject
{
public:
    Way(const std::string &_id, int _type, const std::string &_name);
    virtual ~Way();

    int getPathDirection(Node *_from, Node *_direction);


    // nodes
    Segment* addNode(Node *_node);
    void removeNode(Node *_node);
    int getNodeIndex(Node *_node);
    Node* getNode(int _index);
    Segment* insertNode(Segment *_segment);

    // segments
    void removeSegment(Segment *_segment);
    int getSegmentIndex(Segment *_segment);
    Segment* getSegment(int _index);
    Segment* getSegment(Node *_start, Node *_end);
    void linkSegments();
    void linkLanes();

    bool isBidirectional();
    int getDirectionType();
    int getLaneDirectionType(int _index);
    double getLength();
    double getForwardWidth();
    double getBackwardWidth();
    void setLanes(int _total, int _forward, int _bidirectional, int _backward);

    std::string toString();


    // property accessors
    void setType(int _type);
    void setLanes(int _lanes);
    void setForwardLanes(int _lanes);
    void setBidirectionalLanes(int _lanes);
    void setBackwardLanes(int _lanes);
    void setIsOneway(bool _oneway);
    void setMaxSpeed(double _maxSpeed);


    std::vector<Node*>& getNodes();
    std::vector<Segment*>& getSegments();
    int getLanes();
    int getForwardLanes();
    int getBidirectionalLanes();
    int getBackwardLanes();
    bool isOneway();
    double getMaxSpeed();

protected:
    std::vector<Node*> m_nodes;
    std::vector<Segment*> m_segments;

    int m_lanes;
    int m_forwardLanes;
    int m_bidirectionalLanes;
    int m_backwardLanes;
    bool m_isOneway;

    double m_maxSpeed_mps;
};

}

#endif // LIMOSIM_WAY_H
