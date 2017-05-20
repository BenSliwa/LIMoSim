#include "followpath.h"

namespace LIMoSim
{

FollowPath::FollowPath(Car *_car) :
    MobilityModel(_car)
{
    m_type = "FollowPath";
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void FollowPath::handleNodeReached(Node *_node)
{
    PositionInfo info = p_car->getPositionInfo();
    MobilityUpdate update;
    update.lastNode = _node;



    // select the next 1-hop node from the path and determine a lane to it
    if(info.path.size()>0)
    {
        Node *node = info.path.at(0);
        info.path.erase(info.path.begin());

        info.currentNode = node;
        info.lane = getLaneForNode(info.lane, _node, info.currentNode);
    }
    else // the next 2-hop node is not set yet - determine it
    {
        Segment *segment = info.lane->getSegment();
        Node *previous = segment->getOtherNode(_node);
        std::pair<Node*,int> nextHop = getNextHop(info.lane->getSegment(), info.currentNode, previous);
        info.currentNode = nextHop.first;
        info.lane = getLaneForNode(info.lane, _node, info.currentNode);
    }

    // handle connection lanes
    update.lane = info.lane;
    update.nextNode = info.currentNode;

    if(update.lane->isConnectionLane())
    {
        LaneEndpoint *endpoint = update.lane->getEndEndpoint();

        update.nextNode = endpoint->destination;
        update.lane  = endpoint->gate.out[TURN::THROUGH];
    }

    // determine the next two hop node
    if(info.path.size()==0)
        updatePath(&info, update);


    p_car->setPositionInfo(info);
}

void FollowPath::printPath(const std::vector<Node*> &_path)
{
    std::stringstream stream;
    for(unsigned int i=0; i<_path.size(); i++)
    {
        Node *node = _path.at(i);
        stream << node->getName();
        if(i<_path.size()-1)
            stream << " -> ";
    }

    std::cout << stream.str() << std::endl;
}


}
