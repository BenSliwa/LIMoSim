#ifndef LIMOSIM_INTERSECTIONHANDLING_H
#define LIMOSIM_INTERSECTIONHANDLING_H

#include <vector>
#include "core/mobility/roadperception.h"

namespace LIMoSim
{
class Car;
class Lane;
class Node;
class Segment;


class IntersectionHandling
{
public:
    IntersectionHandling(Car *_car);

    PerceptionEntry getNearestVehicle(const std::vector<Lane*> &_lanes);

    std::vector<Lane*> getConsideredLanes(Node *_node, int _turnIntent, Segment *_segment);
    bool considerSegment(Segment *_from, Segment *_to, int _turnType, int _turnIntent);

    static int getTransitionCategory(int _fromWayType, int _toWayType);
    static int getTurnDirectionCategory(int _turnType);


private:
    Car *p_car;
};

}

#endif // INTERSECTIONHANDLING_H
