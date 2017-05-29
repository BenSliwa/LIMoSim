#include "randomdirection.h"

namespace LIMoSim
{

RandomDirection::RandomDirection(Car *_car) :
    FollowPath(_car)
{
    m_type = "RandomDirection";
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void RandomDirection::updatePath(PositionInfo *_info, const MobilityUpdate &_update)
{
    std::pair<Node*,int> nextHop = getNextHop(_update.lane->getSegment(), _update.nextNode, _update.lastNode);

    _info->turnType = nextHop.second;
    _info->path.push_back(nextHop.first);
}

std::pair<Node*,int> RandomDirection::getNextHop(Segment *_segment, Node *_node, Node *_previous)
{
    std::pair<Node*,int> nextHop;

    // chose the next node
    std::map<int,Node*> opportunities = getOpportunities(_segment, _node);
    std::map<int,Node*>::iterator it;
    int r = RNG::intUniform(0, opportunities.size()-1);
    int i = 0;

    for(it=opportunities.begin(); it!=opportunities.end(); it++)
    {
        int turn = it->first;
        Node *node = it->second;

        if(i==r)
        {
            nextHop.first = node;
            nextHop.second = turn;
            break;
        }
        i++;
    }

    if(!nextHop.first) // loopback
    {
        nextHop.first = _previous;
        nextHop.second = TURN::THROUGH;
    }

    return nextHop;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/



}
