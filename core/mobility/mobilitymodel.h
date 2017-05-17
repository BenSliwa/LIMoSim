#ifndef LIMOSIM_MOBILITYMODEL_H
#define LIMOSIM_MOBILITYMODEL_H

#include <iostream>
#include "core/vehicles/car.h"

#include "core/map/map.h"
#include "core/map/way.h"
#include "core/sim/rng.h"
#include "core/sim/simulation.h"

namespace LIMOSIM
{

class MobilityModel
{
public:
    MobilityModel(Car *_car);

    virtual void handleNodeReached(Node *_node) = 0;

    std::map<int,Node*> getOpportunities(Segment *_segment, Node *_node);
    Lane* getLaneForNode(Lane *_lane, Node *_currentNode, Node *_nextNode);


    //
    std::string getType();

protected:
    Car *p_car;
    std::string m_type;


};


}


#endif // LIMOSIM_MOBILITYMODEL_H
