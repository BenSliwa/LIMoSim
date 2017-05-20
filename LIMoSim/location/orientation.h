#ifndef LIMOSIM_ORIENTATION_H
#define LIMOSIM_ORIENTATION_H

#include "vector3d.h"

namespace LIMoSim
{

class Orientation : public Vector3d
{
public:
    Orientation(double _pitch=0, double _roll=0, double _yaw=0);

    void setPitch(double _pitch);
    void setRoll(double _roll);
    void setYaw(double _yaw);

    double getPitch();
    double getRoll();
    double getYaw();

private:

};

}


#endif // LIMOSIM_ORIENTATION_H
