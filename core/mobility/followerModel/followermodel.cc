#include "followermodel.h"

namespace LIMOSIM
{

FollowerModel::FollowerModel(Car *_car, const std::string &_type) :
    p_car(_car),
    m_type(_type)
{

}

std::string FollowerModel::getType()
{
    return m_type;
}

}
