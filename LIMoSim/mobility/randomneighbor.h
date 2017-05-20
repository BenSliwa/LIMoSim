#ifndef RANDOMNEIGHBOR_H
#define RANDOMNEIGHBOR_H

#include "mobilitymodel.h"

namespace LIMoSim
{

class RandomNeighbor : public MobilityModel
{
public:
    RandomNeighbor(Car *_car);

    void handleNodeReached(Node *_node);

private:
    Lane* getLane(const DestinationEntry &_entry);
};

}


#endif // RANDOMNEIGHBOR_H
