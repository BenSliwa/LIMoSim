#include "position.h"

namespace LIMOSIM
{

Position::Position(double _x, double _y, double _z) : Vector3d(_x, _y, _z)
{

}

Position::Position(const Vector3d &_data) : Vector3d(_data.x, _data.y, _data.z)
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

}
