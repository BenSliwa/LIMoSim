#ifndef LIMOSIM_TRIP_H
#define LIMOSIM_TRIP_H

#include "followpath.h"
#include "LIMoSim/mobility/routing/dijkstra.h"

namespace LIMoSim
{

class Trip : public FollowPath
{
public:
    Trip(Car *_car);

    void addTripEntry(Node *_node);

    void updatePath(PositionInfo *_info, const MobilityUpdate &_update);
    std::pair<Node*,int> getNextHop(Segment *_segment, Node *_node, Node *_previous);

    std::vector<Node*> computePath(Node *_from, Node *_to, Node *_without);

private:
    std::vector<Node*> m_path;
    std::vector<Node*> m_trip;
};

}


#endif // LIMOSIM_TRIP_H
