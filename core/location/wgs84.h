#ifndef LIMOSIM_WGS84_H
#define LIMOSIM_WGS84_H

#include "position.h"
#include "math_.h"

namespace LIMoSim
{

class WGS84
{
public:
    WGS84();

    double getDistance(const Position &_from, const Position &_to);

    Vector3d getOffset(const Position &_node, const Position &_origin);
};

}

#endif // LIMOSIM_WGS84_H
