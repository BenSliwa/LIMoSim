#include "intersectionhandling.h"
#include "LIMoSim/vehicles/car.h"
#include "LIMoSim/map/node.h"
#include "LIMoSim/map/segment.h"
#include "LIMoSim/map/lane.h"
#include "LIMoSim/map/way.h"
#include <sstream>
#include <iostream>

namespace LIMoSim
{

IntersectionHandling::IntersectionHandling(Car *_car) :
    p_car(_car)
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

PerceptionEntry IntersectionHandling::getNearestVehicle(const std::vector<Lane*> &_lanes)
{
    RoadPerception *perception = p_car->getPerception();

    PerceptionEntry nearest;

    for(auto lane : _lanes)
    {
        PerceptionEntry entry = perception->findNearestVehicle(lane, FOLLOWER);

        if(entry.headway_s<nearest.headway_s)
            nearest = entry;
    }

    return nearest;
}

std::vector<Lane*> IntersectionHandling::getConsideredLanes(Node *_node, int _turnIntent, Segment *_segment)
{
    std::stringstream stream;
    stream << "***************" << std::endl;

    SegmentGate *gate = _segment->getGateForNode(_node);
    std::map<int,Segment*>::iterator it;
    std::map<std::pair<Lane*,Lane*>,Lane*>& connectionLanes = _node->getConnectionLanes();

    std::vector<Lane*> consideredLanes;

    for(it=gate->segments.begin(); it!=gate->segments.end(); it++)
    {
        int turnType = it->first;
        Segment *segment = it->second;

        bool consider = considerSegment(_segment, segment, turnType, _turnIntent);

        stream << "from " << _segment->getOtherNode(_node)->getName() << " to " << segment->getOtherNode(_node)->getName();
        stream << "\ttype: " << _node->getTurnTypeDescription(turnType);
        stream << "\tconsider: " << consider << std::endl;



        // get all incoming connection lanes from the segment
        if(consider)
        {
            std::map<std::pair<Lane*,Lane*>,Lane*>::iterator lanes;
            for(lanes=connectionLanes.begin(); lanes!=connectionLanes.end(); lanes++)
            {
                // TODO: add a finer grained decision - a left handed segment maybe be priorizied, but it does not need to be taken into account if it is turning and we are turning right

                Lane *connectionLane = lanes->second;
                if(connectionLane->getSegment()==segment)// && connectionLane->getEndEndpoint()->destination==destination)
                {
                    consideredLanes.push_back(connectionLane);
                    stream << "LANE: " << connectionLane->toString() << std::endl;
                    //std::cout << "D: " << destination->getName() << "\t" << connectionLane->getEndEndpoint()->destination->getName() << std::endl;
                    //std::cout << "type: " << nextNode->getTurnTypeDescription(connectionLane->getTurnType()) << std::endl;
                }
            }
        }

    }

    stream << "LANES: " << consideredLanes.size() << std::endl;

    //std::cout << stream.str() << std::endl;

    return consideredLanes;
}

bool IntersectionHandling::considerSegment(Segment *_from, Segment *_to, int _turnType, int _turnIntent)
{
    bool consider = false;

    int intent = getTurnDirectionCategory(_turnIntent);
    int turn = getTurnDirectionCategory(_turnType);
    int transition = getTransitionCategory(_from->getWay()->getType(), _to->getWay()->getType());

    if(transition==TURN::SUPERIOR)
    {
        if(intent==TURN::LEFT && turn==TURN::THROUGH)
            consider = true;
    }
    else if(transition==TURN::INFERIOR)
    {
        if(intent==TURN::LEFT) // all
            consider = true;
        else if(intent==TURN::THROUGH && turn!=TURN::THROUGH) // left & right
            consider = true;
        else if(intent==TURN::RIGHT && turn==TURN::LEFT) // left
            consider = true;
    }
    else if(transition==TURN::EQUAL)
    {
        if(intent==TURN::LEFT && turn!=TURN::LEFT) // through & right
            consider = true;
        else if(intent==TURN::THROUGH && turn==TURN::RIGHT) // right
            consider = true;
    }

    return consider;
}

int IntersectionHandling::getTransitionCategory(int _fromWayType, int _toWayType)
{
    // TODO: integrate support for more way categories

    if(_fromWayType==WAY_TYPE::PRIMARY_HIGHWAY && _toWayType==WAY_TYPE::RESIDENTIAL)
        return TURN::SUPERIOR;
    else if(_fromWayType==WAY_TYPE::RESIDENTIAL && _toWayType==WAY_TYPE::PRIMARY_HIGHWAY)
        return TURN::INFERIOR;
    return TURN::EQUAL;
}

int IntersectionHandling::getTurnDirectionCategory(int _turnType)
{
    int t = _turnType;
    if(t==TURN::NONE || t==TURN::THROUGH)
        return TURN::THROUGH;
    else if(t==TURN::SLIGHT_LEFT || t==TURN::LEFT || t==TURN::SHARP_LEFT)
        return TURN::LEFT;
    else if(t==TURN::SLIGHT_RIGHT|| t==TURN::RIGHT || t==TURN::SHARP_RIGHT)
        return TURN::RIGHT;
}

}
