#ifndef LIMOSIM_FOLLOWERMODEL_H
#define LIMOSIM_FOLLOWERMODEL_H

#include "LIMoSim/vehicles/car.h"

namespace LIMoSim
{

class FollowerModel
{
public:
    FollowerModel(Car *_car, const std::string &_type);
    virtual ~FollowerModel() {}

    virtual double computeAcceleration(Car *_car) = 0;

    std::string getType();


protected:
    Car *p_car;
    std::string m_type;
};

}


#endif // LIMOSIM_FOLLOWERMODEL_H
