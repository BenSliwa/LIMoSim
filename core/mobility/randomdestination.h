#ifndef LIMOSIM_RANDOMDESTINATION_H
#define LIMOSIM_RANDOMDESTINATION_H

#include "followpath.h"
#include "core/mobility/routing/dijkstra.h"

namespace LIMOSIM
{

class RandomDestination : public FollowPath
{
public:
    RandomDestination(Car *_car);

    void updatePath(PositionInfo *_info, const MobilityUpdate &_update);
    std::pair<Node*,int> getNextHop(Segment *_segment, Node *_node, Node *_previous);

    std::vector<Node*> computeRoutingPath(Node *_from, Node *_to, Node *_without);


private:
    Node* getRandomDestination();
    Lane* getLane(const DestinationEntry &_entry);

    double m_pathStartTime_s;
};

}

#endif // LIMOSIM_RANDOMDESTINATION_H
