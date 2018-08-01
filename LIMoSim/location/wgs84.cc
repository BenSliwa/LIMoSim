#include "wgs84.h"
#include <cmath>
#include <iostream>

namespace LIMoSim
{

WGS84::WGS84()
{

}

double WGS84::getDistance(const Position &_from, const Position &_to)
{
    double n = (_to-_from).norm();

    double lat0 = Math::toRad(_from.y);
    double lon0 = Math::toRad(_from.x);
    double lat1 = Math::toRad(_to.y);
    double lon1 = Math::toRad(_to.x);

    double lambda1 = lon0;
    double lambda2 = lon1;

    double phi1 = lat0;
    double phi2 = lat1;

    double a = cos(phi2)*sin(lambda1-lambda2);
    double b = cos(phi1)*sin(phi2) - sin(phi1) * cos(phi2) * cos(lambda1-lambda2);

    double y = std::sqrt(a*a + b*b);
    double x = sin(phi1) * sin(phi2) + cos(phi1) * cos(phi2) * cos(lambda1-lambda2);

    double angle = atan2(y, x);

    return 6356752.0 * angle;

    /*
    double distance_m = 0;

    double sinTerm = sin(lat0)*sin(lat1);
    double cosTerm = cos(lat0)*cos(lat1)*cos(lon1-lon0);

    distance_m = acos(sinTerm+cosTerm)*40075/360*1000;    // map to earth
    distance_m = Math::toGrad(distance_m);

    return distance_m;
    */
}

Vector3d WGS84::getOffset(const Position &_node, const Position &_origin)
{
    double dX = getDistance(_origin, Position(_node.x, _origin.y));
    double dY = getDistance(_origin, Position(_origin.x, _node.y));
    double dZ = 0;

    return Vector3d(dX, dY, dZ);
}

}

