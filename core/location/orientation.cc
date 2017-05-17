#include "orientation.h"

namespace LIMOSIM
{

Orientation::Orientation(double _pitch, double _roll, double _yaw) :
    Vector3d(_pitch, _roll, _yaw)
{

}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Orientation::setPitch(double _pitch)
{
    x = _pitch;
}

void Orientation::setRoll(double _roll)
{
    y = _roll;
}

void Orientation::setYaw(double _yaw)
{
    z = _yaw;
}

double Orientation::getPitch()
{
    return x;
}

double Orientation::getRoll()
{
    return y;
}

double Orientation::getYaw()
{
    return z;
}



}
