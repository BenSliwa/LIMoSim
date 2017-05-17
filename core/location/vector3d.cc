#include "vector3d.h"

#include <iostream>
#include <sstream>
#include <math.h>

namespace LIMOSIM
{

Vector3d::Vector3d(double _x, double _y, double _z) :
    x(_x),
    y(_y),
    z(_z)
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

double Vector3d::norm()
{
    return sqrt(x*x+y*y+z*z);
}

Vector3d Vector3d::normed()
{
    return *this/norm();
}

Vector3d Vector3d::rotateRight()
{
    return Vector3d(y, -x, z);
}

Vector3d Vector3d::rotateLeft()
{
    return Vector3d(-y, x, z);
}

Vector3d Vector3d::revert()
{
    return Vector3d(-x, -y, z);
}

std::string Vector3d::toString() const
{
    std::stringstream stream;
    stream << x << "," << y << "," << z;

    return stream.str();
}

void Vector3d::info()
{
    std::cout << toString() << std::endl;
}

/*************************************
 *              OPERATORS            *
 ************************************/


Vector3d operator+(Vector3d _lhs, const Vector3d &_rhs)
{
    return Vector3d(_lhs.x+_rhs.x, _lhs.y+_rhs.y, _lhs.z+_rhs.z);
}

Vector3d operator-(Vector3d _lhs, const Vector3d &_rhs)
{
    return Vector3d(_lhs.x-_rhs.x, _lhs.y-_rhs.y, _lhs.z-_rhs.z);
}

Vector3d operator*(Vector3d _lhs, double _rhs)
{
    return Vector3d(_lhs.x*_rhs, _lhs.y*_rhs, _lhs.z*_rhs);
}

Vector3d operator/(Vector3d _lhs, double _rhs)
{
    return Vector3d(_lhs.x/_rhs, _lhs.y/_rhs, _lhs.z/_rhs);
}


}

