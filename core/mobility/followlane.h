#ifndef FOLLOWLANE_H
#define FOLLOWLANE_H

#include "followpath.h"


namespace LIMOSIM
{

class FollowLane : public FollowPath
{
public:
    FollowLane(Car *_car);

    void updatePath(PositionInfo *_info, const MobilityUpdate &_update);
    std::pair<Node*,int> getNextHop(Segment *_segment, Node *_node, Node *_previous);

};



}

#endif // FOLLOWLANE_H
