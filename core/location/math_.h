#ifndef LIMOSIM_MATH_H
#define LIMOSIM_MATH_H

#include "position.h"

namespace LIMOSIM
{

class Math
{
public:
    Math();


    static double computeRotation(const Position &_from, const Position &_to);
    static double getAngleDifference(double _from, double _to);
    static double getMinimumAngleDifference(double _from, double _to);

    static double toGrad(double _value);
    static double toRad(double _value);
    static double pi();

    static bool inInterval(double _value, double _min, double _max);

};

}

#endif // LIMOSIM_MATH_H
