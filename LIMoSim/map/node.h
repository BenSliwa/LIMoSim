#ifndef LIMOSIM_NODE_H
#define LIMOSIM_NODE_H

#include "LIMoSim/location/position.h"
#include "mapobject.h"

#include <map>

namespace LIMoSim
{
class Segment;
class TrafficSignal;
class Way;

struct DestinationEntry
{
    Node *destination = 0;
    Node *neighbor = 0;
    Segment *segment = 0;
    Way *way = 0;
    double cost = 0;
    bool isValid = true;
};



class Node : public MapObject
{
public:
    Node(const Position &_position, const std::string &_id, int _type, const std::string &_name="");
    virtual ~Node();

    void addSegment(Segment *_segment);
    void removeSegment(Segment *_segment);
    int getSegmentIndex(Segment *_segment);
    bool isReachableNeighbor(Node *_node);
    int checkSegmentDirection(Segment *_segment);
    Segment* getOtherSegment(Segment *_segment);


    bool isIntersectionNode();

    void updateConnectionLanes();
    void linkSegments(bool _loopback=true);
    int getTurnType(double _angleDelta);
    std::string getTurnTypeDescription(int _type);
    Lane* getConnectionLane(Lane *_from, Lane *_to);

    void updateDestinations();
    void computeDestinations();
    bool knowsDestination(Node *_node);
    DestinationEntry getBestDestinationEntry(Node *_node, Node *_avoid, bool _incoming=false);

    void updateTrafficSignals();


    std::vector<Segment*> getOutgoingSegments();
    std::vector<Segment*> getIncomingSegments();


    //
    std::string toString();


    // property accessors
    void setPosition(const Position &_position);
    void setTrafficSignal(TrafficSignal *_signal);
    std::vector<Segment*>& getSegments();
    std::vector<DestinationEntry>& getOutgoingConnections();
    std::vector<DestinationEntry>& getIncomingConnections();
    std::map<std::pair<Lane*,Lane*>,Lane*>& getConnectionLanes();

    Position getPosition();
    TrafficSignal* getTrafficSignal();


protected:
    Position m_position;
    TrafficSignal *m_signal;

    std::vector<Segment*> m_segments;
    std::vector<DestinationEntry> m_outgoing;
    std::vector<DestinationEntry> m_incoming;

    std::map<std::pair<Lane*,Lane*>,Lane*> m_connectionLanes;



    // destination,cost
};

}


#endif // LIMOSIM_NODE_H
