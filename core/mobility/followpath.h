#ifndef FOLLOWPATH_H
#define FOLLOWPATH_H

#include "mobilitymodel.h"

namespace LIMoSim
{

struct MobilityUpdate
{
    Lane *lane = 0;
    Node *nextNode = 0;
    Node *lastNode = 0;
};

class FollowPath : public MobilityModel
{

public:
    FollowPath(Car *_car);

    void handleNodeReached(Node *_node);

    virtual void updatePath(PositionInfo *_info, const MobilityUpdate &_update) = 0;
    virtual std::pair<Node*,int> getNextHop(Segment *_segment, Node *_node, Node *_previous) = 0;

protected:
    void printPath(const std::vector<Node*> &_path);

};


}


#endif // FOLLOWPATH_H
