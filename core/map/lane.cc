#include "core/location/math_.h"
#include "lane.h"
#include "segment.h"
#include "mapdefinitions.h"
#include "node.h"
#include <sstream>
#include <iostream>

namespace LIMoSim
{

Lane::Lane(const LaneEndpoint &_start, const LaneEndpoint &_end, Segment *_segment) :
    m_start(_start),
    m_end(_end),
    p_segment(_segment),
    m_index(-1),
    m_directionType(WAY_DIRECTION::FORWARD),
    m_isConnectionLane(false),
    m_turnType(TURN::THROUGH)
{
    m_start.gate.lane = this;
    m_end.gate.lane = this;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

Vector3d Lane::getDirection()
{
    return (m_end.position-m_start.position).normed();
}

double Lane::getLength()
{
    return (m_end.position-m_start.position).norm();
}

double Lane::getRotation()
{
    return Math::computeRotation(m_start.position, m_end.position);
}

LaneGate* Lane::getGateForNode(Node *_node)
{
    LaneEndpoint *endpoint = getEndpointForNode(_node);
    if(endpoint)
        return &endpoint->gate;
    return 0;
}

LaneEndpoint* Lane::getEndpointForNode(Node *_node)
{
    LaneEndpoint *endpoint = 0;
    if(m_start.node==_node)
        endpoint = &m_start;
    else if(m_end.node==_node)
        endpoint = &m_end;

    return endpoint;
}

LaneEndpoint* Lane::getOtherEndpoint(LaneEndpoint *_endpoint)
{
    if(_endpoint==&m_start)
        return &m_end;
    else if(_endpoint==&m_end)
        return &m_start;
    return 0;
}

Node* Lane::getOtherNode(Node *_node)
{
    if(m_start.node==_node)
        return m_end.node;
    else if(m_end.node==_node)
        return m_start.node;
    return 0;
}

void Lane::registerVehicle(Car *_vehicle)
{
    int index = getVehicleIndex(_vehicle);
    if(index==-1)
        m_vehicles.push_back(_vehicle);
}

void Lane::deregisterVehicle(Car *_vehicle)
{
    int index = getVehicleIndex(_vehicle);
    if(index>-1)
        m_vehicles.erase(m_vehicles.begin() + index);
}

int Lane::getVehicleIndex(Car *_vehicle)
{
    int index = -1;
    for(unsigned int i=0; i<m_vehicles.size(); i++)
    {
        if(m_vehicles.at(i)==_vehicle)
        {
            index = i;
            break;
        }
    }

    return index;
}

double Lane::getWidth()
{
    return 3.5;
}

Lane* Lane::getLeftNeighbor()
{
    Lane *neighbor = 0;

    // backward: ++, forward --
    if(m_directionType==WAY_DIRECTION::BACKWARD)
        neighbor = p_segment->getLane(m_index+1);
    else
        neighbor = p_segment->getLane(m_index-1);

    if(neighbor)
    {
        if(neighbor->getDirectionType()!=m_directionType)
            neighbor = 0;
    }

    return neighbor;
}

Lane* Lane::getRightNeighbor()
{
    Lane *neighbor = 0;

    // backward: --, forward ++
    if(m_directionType==WAY_DIRECTION::BACKWARD)
        neighbor = p_segment->getLane(m_index-1);
    else
        neighbor = p_segment->getLane(m_index+1);

    if(neighbor)
    {
        if(neighbor->getDirectionType()!=m_directionType)
            neighbor = 0;
    }

    return neighbor;
}


std::string Lane::toString()
{
    std::stringstream stream;

    if(isConnectionLane())
    {
        stream << "connectionLane: " << m_end.origin->getName() << " -> " << m_end.destination->getName();
    }
    else
        stream << "lane: " << m_start.node->getName() << " -> " << m_end.node->getName();

    return stream.str();
}

void Lane::info()
{
    std::cout << toString() << std::endl;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Lane::setStartEndpoint(const LaneEndpoint &_endpoint)
{
    m_start = _endpoint;
}

void Lane::setEndEndpoint(const LaneEndpoint &_endpoint)
{
    m_end = _endpoint;
}

void Lane::setIndex(int _index)
{
    m_index = _index;
}

void Lane::setSegment(Segment *_segment)
{
    p_segment = _segment;
}

void Lane::setDirectionType(int _type)
{
    m_directionType = _type;
}

void Lane::setIsConnectionLane(bool _value)
{
    m_isConnectionLane = _value;
}

void Lane::setTurnType(int _type)
{
    m_turnType = _type;
}

LaneEndpoint* Lane::getStartEndpoint()
{
    return &m_start;
}

LaneEndpoint* Lane::getEndEndpoint()
{
    return &m_end;
}

Segment* Lane::getSegment()
{
    return p_segment;
}

int Lane::getIndex()
{
    return m_index;
}

int Lane::getDirectionType()
{
    return m_directionType;
}

std::vector<Car*>& Lane::getVehicles()
{
    return m_vehicles;
}

bool Lane::isConnectionLane()
{
    return m_isConnectionLane;
}

int Lane::getTurnType()
{
    return m_turnType;
}

}
