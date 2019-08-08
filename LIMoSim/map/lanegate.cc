#include "lanegate.h"
#include "lane.h"

namespace LIMoSim
{

LaneGate::LaneGate() :
    lane(nullptr)
{

}

Lane* LaneGate::getNextLane()
{
    Lane *lane = nullptr;
    int type = TURN::THROUGH;
    while(out.count(type)==0 && type<TURN::REVERSE)
        type++;

    if(out.count(type)>0)
        lane = out[type];

    return lane;
}

bool LaneGate::isLinkedTo(Lane *_lane)
{
    bool linked = false;
    std::map<int,Lane*>::iterator it;
    for(it=out.begin(); it!=out.end(); it++)
    {
        if(it->second==_lane)
        {
            linked = true;
            break;
        }

    }
    return linked;
}

void LaneGate::clearOut()
{
    // clear all outgoing links and remove the incoming link from the respective lanes
    std::map<int,Lane*>::iterator it;
    for(it=out.begin(); it!=out.end(); it++)
    {
        Lane *lane = it->second;

        LaneGate *inGate = &lane->getStartEndpoint()->gate;
        LaneGate *outGate = &lane->getEndEndpoint()->gate;

        inGate->removeIncoming(lane);
        outGate->removeIncoming(lane);

    }

    //
    out.clear();
}

void LaneGate::addIncomingLane(Lane *_lane, int _type)
{
    in[_lane] = _type;
}

void LaneGate::removeIncoming(Lane *_lane)
{
    std::map<Lane*,int>::iterator it;
    for(it=in.begin(); it!=in.end();)
    {
        Lane *lane = it->first;
        if(lane==_lane)
            in.erase(it++);
        else
            ++it;
    }
}

}
