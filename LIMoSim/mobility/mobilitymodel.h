#ifndef LIMOSIM_MOBILITYMODEL_H
#define LIMOSIM_MOBILITYMODEL_H

#include <iostream>
#include "LIMoSim/vehicles/car.h"

#include "LIMoSim/map/map.h"
#include "LIMoSim/map/way.h"
#include "LIMoSim/sim/rng.h"
#include "LIMoSim/sim/simulation.h"

namespace LIMoSim
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
