#ifndef LIMOSIM_POSITION_H
#define LIMOSIM_POSITION_H

#include "vector3d.h"

namespace LIMoSim
{

class Position : public Vector3d
{
public:
    Position(double _x=0, double _y=0, double _z=0);
    Position(const Vector3d &_data);
};

}

#endif // LIMOSIM_POSITION_H
