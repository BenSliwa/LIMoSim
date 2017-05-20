#ifndef LIMOSIM_CAR_H
#define LIMOSIM_CAR_H

#include "vehicle.h"

#include "LIMoSim/map/lane.h"
#include "LIMoSim/map/way.h"
#include "LIMoSim/map/segment.h"
#include "LIMoSim/map/map.h"
#include "LIMoSim/mobility/roadperception.h"
#include "LIMoSim/mobility/trafficRules/intersectionhandling.h"
#include "LIMoSim/mobility/alignmentcontrol.h"


namespace LIMoSim
{

class FollowerModel;
class LaneChangeModel;
class MobilityModel;
class RandomDestination;
class RandomNeighbor;

struct PositionInfo
{
    Lane *lane = 0;
    Node *currentNode = 0;
    double remainingDistance_m = 0;     // TODO: intersection distance / traffic light distance
    double laneOffset_m = 0;
    double alignment_m = 0;
    int turnType = TURN::THROUGH;
    std::vector<Node*> path;
};

struct BoundingBox
{
    Position frontLeft;
    Position frontRight;
    Position backLeft;
    Position backRight;
};

class Car : public Vehicle
{
public:
    Car(const std::string &_id, const std::string &_name="");
    virtual ~Car();

    virtual void initialize() override;
    virtual void finish();



    void handleEvent(Event *_event);
    double move(double _timeDelta_s);
    void checkCollisions();

    void moveOnLane(double _distance_m);
    void assignLane(Lane *_lane);
    Position computeLanePosition(double _offset_m, double _alignment_m, bool _laneDirection);



    double getDesiredSpeed();
    double getSpeedForTurn(int _type);
    Position getCenter();
    BoundingBox getBoundingBox();

    // control
    void switchToLeftLane();
    void switchToRightLane();


    // property accessors
    void setMobilityModel(MobilityModel *_mobilityModel);
    void setFollowerModel(FollowerModel *_followerModel);
    void setLaneChangeModel(LaneChangeModel *_laneChangeModel);
    void setSpeed(double _speed_mps);
    void setMaxSpeed(double _speed_mps);
    void setSpeedBehaviorFactor(double _factor);
    void setAcceleration(double _acceleration_mpss);
    void setWidth(double _width);
    void setLength(double _length);
    void setPositionInfo(const PositionInfo &_info);

    MobilityModel* getMobilityModel();
    FollowerModel* getFollowerModel();
    LaneChangeModel* getLaneChangeModel();
    RoadPerception* getPerception();
    IntersectionHandling* getIntersectionHandling();
    double getSpeed();
    double getMaxSpeed();
    double getAcceleration();
    double getWidth();
    double getLength();
    PositionInfo getPositionInfo();
    PerceptionMap getPerceptionMap();


private:
    Event *m_movementTimer;
    Event *m_laneChangeTimer;

    MobilityModel *m_mobility;
    RoadPerception *m_perception;
    FollowerModel *m_followerModel;
    LaneChangeModel *m_laneChangeModel;
    IntersectionHandling *m_intersectionHandling;
    AlignmentControl m_alignmentControl;

    double m_speed_mps;
    double m_maxSpeed_mps;
    double m_speedBehaviorFactor;
    double m_acceleration_mpss;

    double m_width;
    double m_length;

    Map *p_map;
    PositionInfo m_positionInfo;
    PerceptionMap m_perceptionMap;


};

}

#endif // LIMOSIM_CAR_H
