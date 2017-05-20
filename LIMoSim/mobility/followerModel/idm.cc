#include "idm.h"
#include <math.h>
#include <limits>
#include "LIMoSim/map/trafficsignal.h"

namespace LIMoSim
{

IDM::IDM(Car *_car) :
    FollowerModel(_car, "IDM"),
    m_safeTimeHeadway_s(1.5),
    m_maxAcceleration_mpss(1),
    m_maxDeceleration_mpss(3), // high impact on the min distance (safety gap before busy road is considered)
    m_accelerationExponent(4.0),
    m_minDistance_m(2.0)
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

double IDM::computeAcceleration(Car *_car)
{

    PositionInfo info = _car->getPositionInfo();
    PerceptionEntry leader = _car->getPerceptionMap().leader;

    double acceleration_mpss = 0;
    if(_car)
        acceleration_mpss = computeAcceleration(_car, leader.vehicle, leader.distance_m, info.lane);


    return acceleration_mpss;
}

double IDM::computeAcceleration(Car *_car, Car *_leader, double _distance_m, Lane *_lane)
{
    PositionInfo info = _car->getPositionInfo();

    double speed_mps = _car->getSpeed();
    double desiredSpeed_mps = _car->getDesiredSpeed();
    double distance_m = -1;
    double speedDelta_mps = 0;

    if(useLeader(_leader))
    {
        distance_m = _distance_m;
        speedDelta_mps = speed_mps - _leader->getSpeed();
    }


    // determine the next traffic signal
    Node *node = info.currentNode;
    double signalDistance_m = std::numeric_limits<double>::max();
    double currentDistance_m = info.remainingDistance_m;
    if(node->getType()!=NODE_TYPE::TRAFFIC_SIGNAL)
    {
        Node *lastNode = info.currentNode;
        for(unsigned int i=0; i<info.path.size(); i++)
        {
            Node *currentNode = info.path.at(i);

            currentDistance_m += (currentNode->getPosition() - lastNode->getPosition()).norm();
            if(currentNode->getType()==NODE_TYPE::TRAFFIC_SIGNAL)
            {
                signalDistance_m = currentDistance_m;

                node = currentNode;
                break;
            }

            lastNode = currentNode;
        }
    }
    else
        signalDistance_m = info.remainingDistance_m;




    if(!_lane->isConnectionLane() && useTrafficSignal(_distance_m, node, signalDistance_m))
    {
        distance_m = signalDistance_m;
        speedDelta_mps = speed_mps;
    }

    //std::cout << "****************** " << p_car->getName() << " ********************" << std::endl;

    PerceptionEntry entry = _car->getPerceptionMap().leader;
    //std::cout << "leader: " << entry.distance_m << std::endl;



    // if we are approaching an intersection and have to respect the driving rules, adjust the desired speed


    /*
    std::vector<Segment*>& segments = info.currentNode->getSegments();
    if(segments.size()>2 ) // the intersection needs to be handled
    {
        //desiredSpeed_mps = 10/3.6;

        std::cout << "s: " << "\td: " << info.remainingDistance_m << std::endl;

        desiredSpeed_mps = std::min(desiredSpeed_mps, (info.remainingDistance_m+5)/3.6);

        distance_m = info.remainingDistance_m + 10;
        speedDelta_mps = speed_mps;
    }*/






   // if(p_car->getId()==0)
    {
        bool d = drive();
        if(!d)
        {
            distance_m = info.remainingDistance_m;
            speedDelta_mps = speed_mps;

            // TODO: drive towards the intersection

            desiredSpeed_mps = 10/3.6;

           // return 0;
        }

    }



    // apply the IDM equation
    double acceleration = computeAcceleration(speed_mps, distance_m, speedDelta_mps, desiredSpeed_mps);

    return acceleration;
}

bool IDM::drive()
{
    bool drive = true;

    PositionInfo info = p_car->getPositionInfo();
    std::vector<Segment*>& segments = info.currentNode->getSegments();
    if(segments.size()>2 && info.remainingDistance_m<50) // the intersection needs to be handled
    {
        IntersectionHandling *intersectionHandling = p_car->getIntersectionHandling();

        // TODO: adjust desired speed

        std::vector<Lane*> lanes = intersectionHandling->getConsideredLanes(info.currentNode, info.turnType, info.lane->getSegment());
        PerceptionEntry entry = intersectionHandling->getNearestVehicle(lanes);

        if(entry.vehicle)
        {

            if(entry.headway_s<6)
            {
                drive = false;
            }

            //std::cout << entry.vehicle->getName() << "\theadway: " << entry.headway_s << "\tdrive: " << drive << "\tdistance: " << info.remainingDistance_m << std::endl;
        }
    }


    return drive;
}

double IDM::computeAcceleration(double _speed_mps, double _distanceToLeader_m, double _speedDelta_mps, double _desiredSpeed_mps)
{
    double freeRoadAcceleration = computeFreeRoadCoefficient(_speed_mps, _desiredSpeed_mps);
    double busyRoadAcceleration = 0;

    /*
     *
     *  TODO: check if -1 is correct
     *
     */



    if(_distanceToLeader_m>-1)
        busyRoadAcceleration = computeBusyRoadCoefficient(_speed_mps, _speedDelta_mps, _distanceToLeader_m);




    double acceleration = m_maxAcceleration_mpss * (1 - freeRoadAcceleration - busyRoadAcceleration);
    acceleration = limitAcceleration(acceleration);

    /*
    std::cout << "free: " << freeRoadAcceleration << "\tbusy: " << busyRoadAcceleration << "\tacc: " << acceleration << std::endl;
    std::cout << "speed: " << p_car->getSpeed()*3.6 << "\tdesired: " << _desiredSpeed_mps*3.6 << "\tacc: " << p_car->getAcceleration() << std::endl;

    std::cout << std::endl;*/

    return acceleration;
}

bool IDM::useLeader(Car *_leader)
{
    if(_leader)
        return true;
    return false;
}

bool IDM::useTrafficSignal(double _leaderDistance_m, Node *_node, double _distance_m)
{
    //
    if(_leaderDistance_m<_distance_m)
        return false;


    // TODO: get next traffic signal with perception!
    if(_node->getType()==NODE_TYPE::TRAFFIC_SIGNAL)
    {
        TrafficSignal *signal = _node->getTrafficSignal();
        int direction = signal->getDirection();

        // only handle the traffic signal if the direction requires it
        /*
         * Segment *segment = _info.lane->getSegment();
        SegmentGate *gate = segment->getGateForNode(node);
        if(gate==segment->getEndGate() && direction==WAY_DIRECTION::BACKWARD)
            return false;
        if(gate==segment->getStartGate() && direction==WAY_DIRECTION::FORWARD)
            return false;
        */

        // TODO: get remaining state time


        // handle the traffic signal state
        int state = signal->getState();
        if(state==TRAFFIC_SIGNAL::RED || state==TRAFFIC_SIGNAL::YELLOW)
            return true;
    }
    return false;
}

double IDM::limitAcceleration(double _acceleration)
{
    double acceleration = _acceleration;
    if(acceleration<-m_maxDeceleration_mpss)
        acceleration = -m_maxDeceleration_mpss;
    else if(acceleration>m_maxAcceleration_mpss)
        acceleration = m_maxAcceleration_mpss;

    return acceleration;
}

double IDM::computeFreeRoadCoefficient(double _speed_mps, double _desiredSpeed_mps)
{
    double coefficient = pow((_speed_mps/_desiredSpeed_mps), m_accelerationExponent);
    if(_desiredSpeed_mps==0)
        coefficient = 1;

    return coefficient;
}

double IDM::computeBusyRoadCoefficient(double _speed_mps, double _speedDelta_mps, double _distance_m)
{
    double desiredGap_m = computeDesiredGap(_speed_mps, _speedDelta_mps);
    double coefficient = pow((desiredGap_m/_distance_m), 2);

    /*
    std::cout << "busycoeff: " << desiredGap_m << "\t" << coefficient << "\t";
    std::cout << _speedDelta_mps << "\t" << _distance_m << "\t";
    std::cout << std::endl;
    */

    return coefficient;
}

double IDM::computeDesiredGap(double _speed_mps, double _speedDelta_mps)
{
    // caution: s0 + s1 in reality
    double breakGap_m = computeBreakGap(_speed_mps, _speedDelta_mps);
    double timeGap_m = _speed_mps * m_safeTimeHeadway_s;
    double distance_m = m_minDistance_m + timeGap_m + breakGap_m;

    return distance_m;
}

double IDM::computeBreakGap(double _speed_mps, double _speedDelta_mps)
{
    double distance_m = _speed_mps * _speedDelta_mps / (2 * sqrt(m_maxAcceleration_mpss * m_maxDeceleration_mpss));

    return distance_m;
}


void IDM::setSafeTimeheadway(double _headway_s)
{
    m_safeTimeHeadway_s = _headway_s;
}

void IDM::setMaxAcceleration(double _acceleration)
{
    m_maxAcceleration_mpss = _acceleration;
}

void IDM::setMaxDeceleration(double _deceleration)
{
    m_maxDeceleration_mpss = _deceleration;
}

void IDM::setAccelerationExponent(double _exponent)
{
    m_accelerationExponent = _exponent;
}

void IDM::setMinDistance(double _distance_m)
{
    m_minDistance_m = _distance_m;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/


}
