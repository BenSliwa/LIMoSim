#include "wgs84.h"
#include <math.h>
#include <iostream>

namespace LIMOSIM
{

WGS84::WGS84()
{

}

double WGS84::getDistance(const Position &_from, const Position &_to)
{
    double n = (_to-_from).norm();

    double distance_m = 0;

    double lat0 = Math::toRad(_from.y);
    double lon0 = Math::toRad(_from.x);
    double lat1 = Math::toRad(_to.y);
    double lon1 = Math::toRad(_to.x);

    double sinTerm = sin(lat0)*sin(lat1);
    double cosTerm = cos(lat0)*cos(lat1)*cos(lon1-lon0);

    distance_m = acos(sinTerm+cosTerm)*40000/360*1000;    // map to earth
    distance_m = Math::toGrad(distance_m);

    return distance_m;
}

Vector3d WGS84::getOffset(const Position &_node, const Position &_origin)
{
    double dX = getDistance(_origin, Position(_node.x, _origin.y));
    double dY = getDistance(_origin, Position(_origin.x, _node.y));
    double dZ = 0;

    return Vector3d(dX, dY, dZ);
}

}

