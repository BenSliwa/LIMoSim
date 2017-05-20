#include "LIMoSim/location/math_.h"
#include <math.h>
#include "car.h"
#include "LIMoSim/sim/rng.h"
#include "LIMoSim/mobility/mobilitymodels.h"
#include "LIMoSim/mobility/followerModel/idm.h"
#include "LIMoSim/mobility/laneChange/lanechangemodels.h"
#include <algorithm>


namespace LIMoSim
{

Car::Car(const std::string &_id, const std::string &_name) :
    Vehicle(_id, _name),
    m_movementTimer(0),
    m_laneChangeTimer(0),
    m_mobility(0),
    m_perception(new RoadPerception(this)),
    m_followerModel(0),
    m_laneChangeModel(0),
    m_intersectionHandling(new IntersectionHandling(this)),
    m_speed_mps(0),
    m_maxSpeed_mps(130/3.6),
    m_speedBehaviorFactor(1),
    m_acceleration_mpss(0),
    m_width(2),
    m_length(5),
    p_map(Map::getInstance())
{
    // statistics
    if(m_id=="0")
    {
        Statistics *statistics = StatisticsManager::getInstance()->createStatistics(m_name);
        statistics->addKey("x");
        statistics->addKey("y");
        statistics->addKey("distance");
        statistics->addKey("speed");
        statistics->addKey("acceleration");
    }
}

Car::~Car()
{
    deleteEvent(m_laneChangeTimer);
    deleteEvent(m_movementTimer);

    delete m_laneChangeModel;
    delete m_followerModel;
    delete m_intersectionHandling;
    delete m_perception;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Car::initialize()
{
    std::cout << "Car::initialize" << std::endl;

    m_updateInterval_s = 25.0/1000.0;
    m_movementTimer = new Event(m_updateInterval_s, this, "Car::updateTimer");
    scheduleEvent(m_movementTimer);

    m_laneChangeTimer = new Event(1, this, "Car::laneChangeTimer");
    scheduleEvent(m_laneChangeTimer);
}

void Car::finish()
{
    std::cout << "Car::finish" << std::endl;
}

void Car::handleEvent(Event *_event)
{

    if(_event==m_movementTimer)
    {
        double delta_s = _event->getTimestamp() - m_lastUpdate_s;
        m_lastUpdate_s = _event->getTimestamp();

        double distance_m = move(delta_s);

        scheduleEvent(m_movementTimer, m_updateInterval_s);


        // update statistics
        Statistics *statistics = StatisticsManager::getInstance()->getStatistics(m_name);
        if(statistics)
        {
            statistics->add(StatisticsEntry("x", m_position.x, Stats::LAST));
            statistics->add(StatisticsEntry("y", m_position.y, Stats::LAST));
            statistics->add(StatisticsEntry("distance", distance_m, Stats::SUM));
            statistics->add(StatisticsEntry("speed", m_speed_mps*3.6, Stats::MEAN));
            statistics->add(StatisticsEntry("acceleration", m_acceleration_mpss, Stats::MEAN));
        }
    }
    else if(_event==m_laneChangeTimer)
    {
        if(getId()=="0")
        {
            if(m_alignmentControl.getState()==ALIGN::IDLE)
            {
                double leftIncentive = m_laneChangeModel->computeLeftLaneIncentive();
                double rightIncentive = m_laneChangeModel->computeRightLaneIncentive();

                //std::cout << leftIncentive << "\t" << rightIncentive << std::endl;

                if(leftIncentive>0.1)
                    switchToLeftLane();
                else if(rightIncentive>0.1)
                    switchToRightLane();
            }



        }

        scheduleEvent(m_laneChangeTimer, 1);
    }
}

double Car::move(double _timeDelta_s)
{
    if(!m_positionInfo.currentNode)
    {
        Lane *lane = m_positionInfo.lane;

        if(lane->getDirectionType()==WAY_DIRECTION::BACKWARD)
            m_positionInfo.currentNode = lane->getEndEndpoint()->node;
        else
            m_positionInfo.currentNode = lane->getStartEndpoint()->node;
    }


    double distance_m = m_speed_mps * _timeDelta_s;

   // if(m_maxSpeed_mps>0)
    {

        // update the road perception
        m_perceptionMap = m_perception->update();

        // compute the new acceleration and update the speed
        m_acceleration_mpss = m_followerModel->computeAcceleration(this);

        if(m_maxSpeed_mps>0)
            m_speed_mps += (m_acceleration_mpss * _timeDelta_s);



    }


    // perform the actual movement
    moveOnLane(distance_m);

    //checkCollisions();

    return distance_m;
}

void Car::checkCollisions()
{
    std::map<std::string,Car*>& cars = p_map->getCars();
    std::map<std::string,Car*>::iterator c;

    for(c=cars.begin(); c!=cars.end(); c++)
    {
        Car* car = c->second;
        if(car!=this)
        {
            BoundingBox box = car->getBoundingBox();
            std::vector<Position> positions;
            positions.push_back(box.backLeft);
            positions.push_back(box.backRight);
            positions.push_back(box.frontLeft);
            positions.push_back(box.frontRight);

            for(unsigned int i=0; i<positions.size(); i++)
            {
                Position p = positions.at(i);
                double x = p.x - m_position.x;
                double y = p.y - m_position.y;

                double phi = m_orientation.getYaw() * Math::pi() / 180;
                double x_r = x * cos(phi) + y * sin(phi);
                double y_r = -x * sin(phi) + y * cos(phi);

                if(fabs(x_r)<=0.5 && fabs(y_r)<=1)
                {
                    //std::cout << "COLLISION " << x_r << "\t" << y_r << std::endl;
                    //p_simulation->getEventScheduler()->stop();
                }
            }
        }
    }




}

void Car::moveOnLane(double _distance_m)
{

    double m_remainingDistance_m = m_positionInfo.lane->getLength() - m_positionInfo.laneOffset_m;

    if(m_remainingDistance_m<_distance_m) // lane switch
    {
        //std::cout << getName() << " switch lane" << std::endl;
        Node *reachedNode = m_positionInfo.currentNode;

        Lane *lane = m_positionInfo.lane;

        if(!lane->isConnectionLane())
        {

            m_mobility->handleNodeReached(reachedNode);
        }
        else
        {


            Node *destination = lane->getEndEndpoint()->destination;
            Node *origin = lane->getEndEndpoint()->origin;

            lane = lane->getEndEndpoint()->gate.getNextLane();



            if(!lane->isConnectionLane())
                m_positionInfo.currentNode = destination;



            assignLane(lane);



        }


        m_positionInfo.laneOffset_m = 0;


       // std::cout << "switch: " << reachedNode->getName() << " -> " << m_positionInfo.currentNode->getName() << std::endl;

    }
    else
    {
        m_positionInfo.laneOffset_m += _distance_m;
    }




    m_positionInfo.remainingDistance_m = m_positionInfo.lane->getLength() - m_positionInfo.laneOffset_m;
    m_orientation.setYaw(m_positionInfo.lane->getRotation());

    m_positionInfo.alignment_m = m_alignmentControl.updateLateralPosition(m_positionInfo.lane, m_positionInfo.alignment_m, this);
    m_position = computeLanePosition(m_positionInfo.laneOffset_m, m_positionInfo.alignment_m, true);
}

void Car::assignLane(Lane *_lane)
{
    m_positionInfo.lane->deregisterVehicle(this);
    _lane->registerVehicle(this);
    m_positionInfo.lane = _lane;
}

Position Car::computeLanePosition(double _offset_m, double _alignment_m, bool _laneDirection)
{
    Position position;
    Lane *lane = m_positionInfo.lane;

    if(_laneDirection)
        position = lane->getStartEndpoint()->position + lane->getDirection() * _offset_m;
    else
        position = lane->getEndEndpoint()->position - lane->getDirection() * _offset_m;


     //_alignment_m  = 3.5;
    // vertical position



    position = position + (m_position-getCenter()).rotateRight().normed() * _alignment_m;


    return position;
}

double Car::getDesiredSpeed()
{
    // compute the desired speed from the vehicle's capabilities, the driver behaviror and the road limit
    double allowedSpeed_mps = m_positionInfo.lane->getSegment()->getWay()->getMaxSpeed();
    double desiredSpeed_mps = std::min(allowedSpeed_mps, m_maxSpeed_mps);

    //
    int turn = m_positionInfo.turnType;
    if(m_positionInfo.lane->isConnectionLane())
        turn = m_positionInfo.lane->getTurnType();
    if(turn!=TURN::THROUGH && m_positionInfo.remainingDistance_m<20)
    {
        desiredSpeed_mps = getSpeedForTurn(turn);

        //desiredSpeed_mps = 1/3.6;
    }

   // std::cout << "approaching: " << m_positionInfo.remainingDistance_m << "\t" << m_speed_mps << "\t" << m_acceleration_mpss << std::endl;


    return desiredSpeed_mps * m_speedBehaviorFactor;
}

double Car::getSpeedForTurn(int _type)
{
    double speed_kmh = 50;
    if(_type==TURN::LEFT || _type==TURN::RIGHT)
    {
        speed_kmh = 20;
    }
    else if(_type==TURN::SHARP_LEFT || _type==TURN::SHARP_RIGHT)
    {
        speed_kmh = 10;
    }

    return speed_kmh/3.6;
}

Position Car::getCenter()
{
    double theta = Math::pi()/2;
    double phi = (m_orientation.getYaw() - 180) * Math::pi() / 180;

    double x  = m_length * sin(theta) * cos(phi) + m_position.x;
    double y =  m_length * sin(theta) * sin(phi) + m_position.y;

    return (Position(x,y) + m_position)/2;
}

BoundingBox Car::getBoundingBox()
{
    Position front = m_position;
    Position center = getCenter();
    Position back = center + center - front;

    Position dir = (front-back).normed();
    dir = dir.rotateRight();

    double w = m_width / 2;

    BoundingBox box;
    box.frontLeft = front - dir * w;
    box.frontRight = front + dir * w;
    box.backLeft = back - dir * w;
    box.backRight = back + dir * w;

    return box;
}

void Car::switchToLeftLane()
{
    m_alignmentControl.changeLane(ALIGN::CHANGE_TO_LEFT, m_updateInterval_s);
}

void Car::switchToRightLane()
{
    m_alignmentControl.changeLane(ALIGN::CHANGE_TO_RIGHT, m_updateInterval_s);
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Car::setMobilityModel(MobilityModel *_mobilityModel)
{
    m_mobility = _mobilityModel;
}

void Car::setFollowerModel(FollowerModel *_followerModel)
{
    m_followerModel = _followerModel;
}

void Car::setLaneChangeModel(LaneChangeModel *_laneChangeModel)
{
    m_laneChangeModel = _laneChangeModel;
}

void Car::setSpeed(double _speed_mps)
{
    m_speed_mps = _speed_mps;
}

void Car::setMaxSpeed(double _speed_mps)
{
    m_maxSpeed_mps = _speed_mps;
}

void Car::setSpeedBehaviorFactor(double _factor)
{
    m_speedBehaviorFactor = _factor;
}

void Car::setAcceleration(double _acceleration_mpss)
{
    m_acceleration_mpss = _acceleration_mpss;
}

void Car::setWidth(double _width)
{
    m_width = _width;
}

void Car::setLength(double _length)
{
    m_length = _length;
}

void Car::setPositionInfo(const PositionInfo &_info)
{
    Lane *lane = m_positionInfo.lane;
    if(_info.lane!=lane) // handle lane switch
    {
        // deregister the vehicle with the new lane
        if(lane)
            lane->deregisterVehicle(this);

        // register the vehicle with the new lane
        _info.lane->registerVehicle(this);
    }


    m_positionInfo = _info;
}

MobilityModel* Car::getMobilityModel()
{
    return m_mobility;
}

FollowerModel* Car::getFollowerModel()
{
    return m_followerModel;
}

LaneChangeModel* Car::getLaneChangeModel()
{
    return m_laneChangeModel;
}

RoadPerception* Car::getPerception()
{
    return m_perception;
}

IntersectionHandling* Car::getIntersectionHandling()
{
    return m_intersectionHandling;
}

double Car::getSpeed()
{
    return m_speed_mps;
}

double Car::getMaxSpeed()
{
    return m_maxSpeed_mps;
}

double Car::getAcceleration()
{
    return m_acceleration_mpss;
}

double Car::getWidth()
{
    return m_width;
}

double Car::getLength()
{
    return m_length;
}

PositionInfo Car::getPositionInfo()
{
    return m_positionInfo;
}

PerceptionMap Car::getPerceptionMap()
{
    return m_perceptionMap;
}


}
