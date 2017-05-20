#ifndef LIMOSIM_LANE_H
#define LIMOSIM_LANE_H

#include "LIMoSim/map/mapdefinitions.h"
#include "LIMoSim/location/position.h"
#include <map>
#include <vector>
#include "lanegate.h"

namespace LIMoSim
{

class Car;
class Lane;
class Node;
class Segment;

namespace LANE_GATE
{
    enum{
        START = 0,
        END = 1
    };
}

struct LaneEndpoint
{
    Node *node = 0;
    Position position;
    LaneGate gate;
    Node *destination = 0; // for connection lanes
    Node *origin = 0;
};


class Lane
{
public:
    Lane(const LaneEndpoint &_start, const LaneEndpoint &_end, Segment *_segment);

    Vector3d getDirection();
    double getLength();
    double getRotation();
    LaneGate* getGateForNode(Node *_node);
    LaneEndpoint* getEndpointForNode(Node *_node);
    LaneEndpoint* getOtherEndpoint(LaneEndpoint *_endpoint);
    Node* getOtherNode(Node *_node);

    //
    void registerVehicle(Car *_vehicle);
    void deregisterVehicle(Car *_vehicle);
    int getVehicleIndex(Car *_vehicle);

    //
    double getWidth();
    Lane* getLeftNeighbor();
    Lane* getRightNeighbor();

    //
    std::string toString();
    void info();

    // property accessors
    void setStartEndpoint(const LaneEndpoint &_endpoint);
    void setEndEndpoint(const LaneEndpoint &_endpoint);
    void setSegment(Segment *_segment);
    void setIndex(int _index);
    void setDirectionType(int _type);
    void setIsConnectionLane(bool _value);
    void setTurnType(int _type);

    LaneEndpoint* getStartEndpoint();
    LaneEndpoint* getEndEndpoint();
    Segment* getSegment();
    int getIndex();
    int getDirectionType();
    std::vector<Car*>& getVehicles();
    bool isConnectionLane();
    int getTurnType();

private:
    LaneEndpoint m_start;
    LaneEndpoint m_end;

    Segment *p_segment;

    int m_index;
    int m_directionType;

    std::vector<Car*> m_vehicles;

    bool m_isConnectionLane;
    int m_turnType;

};

}

#endif // LIMOSIM_LANE_H
