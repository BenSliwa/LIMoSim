#ifndef LIMOSIM_ROAD_H
#define LIMOSIM_ROAD_H

#include "LIMoSim/map/way.h"

namespace LIMoSim
{

class Road : public Way
{
public:
    Road(const std::string &_id, int _type, const std::string &_name);

protected:
    int m_lanes;
    int m_forwardLanes;

};

}

#endif // LIMOSIM_ROAD_H
