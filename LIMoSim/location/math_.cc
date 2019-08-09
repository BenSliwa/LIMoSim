#include <cmath>
#include "math_.h"
#include <algorithm>

namespace LIMoSim
{

Math::Math()
{

}

double Math::computeRotation(const Position &_from, const Position &_to)
{
    Position direction = _to - _from;
    double rotation = toGrad(atan2(direction.y, direction.x));
    return rotation;
}


double Math::getAngleDifference(double _from, double _to)
{
    double d = _to - _from;
    if(std::abs(d)>180)
    {
        d = std::min(_from, _to) + 360 - std::max(_from, _to);
        if(_from<_to)
            d *= -1;
    }

    return d;
}

double Math::getMinimumAngleDifference(double _from, double _to)
{
    double angleDifference = getAngleDifference(_from, _to);
    if(angleDifference>90)
        angleDifference = 180 - angleDifference;
    else if(angleDifference<-90)
        angleDifference = angleDifference + 180;

    return angleDifference;
}

double Math::toGrad(double _value)
{
    return _value * 180 / pi();
}

double Math::toRad(double _value)
{
    return _value * pi() / 180;
}

double Math::pi()
{
    return 3.14159265359;
}

bool Math::inInterval(double _value, double _min, double _max)
{
    if(_value>=_min && _value<=_max)
        return true;
    return false;
}


}
