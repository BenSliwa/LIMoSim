#ifndef LIMOSIM_RANDOMDIRECTION_H
#define LIMOSIM_RANDOMDIRECTION_H

#include "followpath.h"


namespace LIMoSim
{

class RandomDirection : public FollowPath
{
public:
    RandomDirection(Car *_car);

    void updatePath(PositionInfo *_info, const MobilityUpdate &_update);
    std::pair<Node*,int> getNextHop(Segment *_segment, Node *_node, Node *_previous);

};



}

#endif // LIMOSIM_RANDOMDIRECTION_H
