#include "roadperception.h"
#include "LIMoSim/vehicles/car.h"
#include "LIMoSim/map/mapdefinitions.h"

namespace LIMoSim
{

RoadPerception::RoadPerception(Car *_car) :
    p_car(_car),
    m_range_m(100)
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

PerceptionMap RoadPerception::update()
{
    PositionInfo info = p_car->getPositionInfo();
    Lane *lane = info.lane;

    PerceptionMap map;
    map.leader = getLeader(lane);
    map.follower = getFollower(lane);

    //std::cout << map.leader.distance_m << std::endl;

    return map;
}

PerceptionEntry RoadPerception::getLeader(Lane *_lane)
{
    return findNearestVehicle(_lane, LEADER);
}

PerceptionEntry RoadPerception::getFollower(Lane *_lane)
{
    return findNearestVehicle(_lane, FOLLOWER);
}

PerceptionEntry RoadPerception::findNearestVehicle(Lane *_lane, int _type)
{
    PerceptionEntry entry;;

    if(_lane)
    {
        Lane *lane = _lane;
        double offsetOnlane_m = p_car->getPositionInfo().laneOffset_m;
        double laneOffset_m = 0;

        if(_type==FOLLOWER)
            offsetOnlane_m = lane->getLength() - offsetOnlane_m;

        // start with the current lane and loop over all following lanes until the max distance is reached
        //std::cout << "**************************" << std::endl;
        while(lane && laneOffset_m<=m_range_m)
        {
            //lane->info();

            entry = findNearestVehicleOnLane(lane, offsetOnlane_m, laneOffset_m, _type);
            if(entry.vehicle)
                break;

            //
            laneOffset_m += lane->getLength()-offsetOnlane_m;
            lane = getNextLane(lane, _type);
            offsetOnlane_m = 0; // only required for the first lane
        }
    }

    return entry;
}

PerceptionEntry RoadPerception::findNearestVehicleOnLane(Lane *_lane,  double _offsetOnlane_m, double _laneOffset_m, int _type)
{
    PerceptionEntry entry;

    std::vector<Car*> cars =  _lane->getVehicles();
    for(auto car : cars)
    {
        double offset_m = getOffsetOnLane(car, _lane, _type);
        if(car!=p_car && offset_m>=_offsetOnlane_m)
        {
            double totalOffset_m = offset_m + _laneOffset_m - _offsetOnlane_m;
            if(_type==LEADER)
                totalOffset_m -= car->getLength();
            else if(_type==FOLLOWER)
                totalOffset_m -= p_car->getLength();

            if(totalOffset_m<m_range_m && totalOffset_m<entry.distance_m)
            {
                entry.vehicle = car;
                entry.distance_m = totalOffset_m;
                entry.headway_s = entry.distance_m / car->getSpeed();
                entry.speed_mps = car->getSpeed();
                // TODO: entry.headway_s =
            }
        }
    }

    return entry;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

double RoadPerception::getOffsetOnLane(Car *_car, Lane *_lane, int _type)
{
    double offset_m = _car->getPositionInfo().laneOffset_m;
    if(_type==FOLLOWER)
        offset_m = _lane->getLength() - offset_m;

    return offset_m;
}

Lane* RoadPerception::getNextLane(Lane *_lane, int _type)                       // caution: gate selection
{
    Lane *lane = nullptr;
    if(_type==LEADER)
    {
        LaneGate *gate = &_lane->getEndEndpoint()->gate;
        lane = gate->getNextLane();
    }
    else if(_type==FOLLOWER)
    {
        LaneGate *gate = &_lane->getStartEndpoint()->gate;

        std::map<Lane*,int>::iterator it;
        for(it=gate->in.begin(); it!=gate->in.end(); it++)
        {
            Lane *inLane = it->first;
            if(inLane)
            {
                lane = inLane;
                break;
            }
        }
    }

    return lane;
}


}
