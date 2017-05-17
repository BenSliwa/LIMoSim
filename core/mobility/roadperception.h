#ifndef LIMOSIM_ROADPERCEPTION_H
#define LIMOSIM_ROADPERCEPTION_H

#include <limits>

namespace LIMOSIM
{
class Car;
class Lane;

enum{
    LEADER,
    FOLLOWER
};

struct PerceptionEntry
{
    Car *vehicle = 0;
    double distance_m = std::numeric_limits<double>::max();
    double headway_s = std::numeric_limits<double>::max();
    double speed_mps = 0;
};

struct PerceptionMap
{
    PerceptionEntry leader;
    PerceptionEntry follower;
};

class RoadPerception
{
public:
    RoadPerception(Car *_car);



    PerceptionMap update();
    PerceptionEntry getLeader(Lane *_lane);
    PerceptionEntry getFollower(Lane *_lane);

    PerceptionEntry findNearestVehicle(Lane *_lane, int _type);
    PerceptionEntry findNearestVehicleOnLane(Lane *_lane,  double _offsetOnlane_m, double _laneOffset_m, int _type);

private:
    double getOffsetOnLane(Car *_car, Lane *_lane, int _type);
    Lane* getNextLane(Lane *_lane, int _type);

private:
    Car *p_car;
    double m_range_m;
};


}


#endif // LIMOSIM_ROADPERCEPTION_H
