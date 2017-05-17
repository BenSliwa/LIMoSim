#ifndef LIMOSIM_VECTOR3D_H
#define LIMOSIM_VECTOR3D_H

#include <string>

namespace LIMOSIM
{

class Vector3d
{
public:
    Vector3d(double _x=0, double _y=0, double _z=0);

    double norm();
    Vector3d normed();
    Vector3d rotateRight();
    Vector3d rotateLeft();
    Vector3d revert();

    //
    std::string toString() const;
    void info();

public:
    double x;
    double y;
    double z;
};

Vector3d operator+(Vector3d _lhs, const Vector3d &_rhs);
Vector3d operator-(Vector3d _lhs, const Vector3d &_rhs);
Vector3d operator*(Vector3d _lhs, double _rhs);
Vector3d operator/(Vector3d _lhs, double _rhs);

}

#endif // LIMOSIM_VECTOR3D_H
