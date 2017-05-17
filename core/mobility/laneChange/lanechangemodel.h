#ifndef LIMOSIM_LANECHANGEMODEL_H
#define LIMOSIM_LANECHANGEMODEL_H

#include "core/vehicles/car.h"

namespace LIMoSim
{

class LaneChangeModel
{
public:
    LaneChangeModel(Car *_car, const std::string &_type);

    virtual double computeLeftLaneIncentive() = 0;
    virtual double computeRightLaneIncentive() = 0;

    std::string getType();

protected:
    Car *p_car;
    std::string m_type;
};

}

#endif // LANECHANGEMODEL_H
