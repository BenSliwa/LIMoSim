#include "randomneighbor.h"

namespace LIMOSIM
{

RandomNeighbor::RandomNeighbor(Car *_car) :
    MobilityModel(_car)
{
    m_type = "RandomNeighbor";
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void RandomNeighbor::handleNodeReached(Node *_node)
{
    // determine a random neighbor
    std::vector<DestinationEntry>& outgoing = _node->getOutgoingConnections();
    int index = RNG::intUniform(0, outgoing.size()-1);
    DestinationEntry entry = outgoing.at(index);

    // update the position info
    PositionInfo info = p_car->getPositionInfo();
    info.lane = getLane(entry);
    info.currentNode = entry.neighbor;
    p_car->setPositionInfo(info);

    std::cout << "RandomNeighbor::handleNodeReached " << _node->getName() << "\tnext: " << info.currentNode->getName() << std::endl;

}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

Lane* RandomNeighbor::getLane(const DestinationEntry &_entry)
{
    Node *nextNode = _entry.neighbor;
    Segment *segment = _entry.segment;
    Lane *lane = 0;

    if(nextNode==segment->getEndGate()->node) // with segment direction
        lane = segment->getLanes().at(1);
    else
        lane = segment->getLanes().at(0);

    return lane;
}


/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/


}
