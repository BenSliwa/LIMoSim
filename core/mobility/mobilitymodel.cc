#include "mobilitymodel.h"

namespace LIMoSim
{

MobilityModel::MobilityModel(Car *_car) :
    p_car(_car)
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


std::map<int,Node*> MobilityModel::getOpportunities(Segment *_segment, Node *_node)
{
    std::map<int,Node*> opportunities;

    SegmentGate *gate = _segment->getGateForNode(_node);
    std::map<int,Segment*>::iterator it;
    for(it=gate->segments.begin(); it!=gate->segments.end(); it++)
    {
        int turnType = it->first;
        Segment* nextSegment = it->second;
        Node* node = 0;

        node = nextSegment->getOtherNode(_node);

        opportunities[turnType] = node;
    }

    return opportunities;
}


Lane* MobilityModel::getLaneForNode(Lane *_lane, Node *_currentNode, Node *_nextNode)
{
    Lane *lane = 0;
    LaneGate *gate = _lane->getGateForNode(_currentNode);

    std::map<int,Lane*>::iterator out;
    for(out=gate->out.begin(); out!=gate->out.end(); out++)
    {
        Lane *nextLane = out->second;
        Node *next = nextLane->getOtherNode(_currentNode);
        if(nextLane->isConnectionLane())
            next = nextLane->getEndEndpoint()->destination;

        if(next==_nextNode)
        {
            lane = nextLane;
            break;
        }
    }

    return lane;
}



/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

std::string MobilityModel::getType()
{
    return m_type;
}

}
