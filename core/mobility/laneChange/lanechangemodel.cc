#include "lanechangemodel.h"

namespace LIMOSIM
{

LaneChangeModel::LaneChangeModel(Car *_car, const std::string &_type) :
    p_car(_car),
    m_type(_type)
{

}

std::string LaneChangeModel::getType()
{
    return m_type;
}

}
