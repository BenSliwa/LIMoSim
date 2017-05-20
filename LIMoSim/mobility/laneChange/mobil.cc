#include "mobil.h"
#include <algorithm>

namespace LIMoSim
{

MOBIL::MOBIL(Car *_car) :
    LaneChangeModel(_car, "MOBIL"),
    p_perception(p_car->getPerception()),
    p_idm(static_cast<IDM*>(p_car->getFollowerModel())),
    m_maxDeceleration_mpss(4),
    m_politeness(0.7),
    m_changingThreshold_mpss(0.1),
    m_rightLaneBias_mpss(0.3)
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


double MOBIL::computeLeftLaneIncentive()
{
    double incentive = 0;

    PositionInfo info = p_car->getPositionInfo();
    Lane *left = info.lane->getLeftNeighbor();
    PerceptionEntry follower = p_perception->getFollower(info.lane);



    bool safe = checkSafetyCondition(left, follower);
    if(safe)
    {

        double a_c0 = p_idm->computeAcceleration(p_car);
        double a_c1 = p_idm->computeAcceleration(p_car, follower.vehicle, follower.distance_m, left);

        PerceptionEntry leader = p_perception->getLeader(info.lane);
        double a_eur = a_c0;
        if(leader.vehicle)
        {
            if(p_car->getSpeed() > leader.speed_mps)
                a_eur = std::min(a_c0, a_c1);
        }


        double a_n0 = 0;
        double a_n1 = 0;
        PerceptionEntry leftFollower = p_perception->getFollower(left);
        if(leftFollower.vehicle)
        {
            a_n0 = p_idm->computeAcceleration(leftFollower.vehicle);
            a_n1 = p_idm->computeAcceleration(leftFollower.vehicle, p_car, leftFollower.distance_m, left);
        }


        double changeTerm = a_c1 - a_eur + m_politeness * (a_n1 - a_n0);
        double biasTerm = m_changingThreshold_mpss + m_rightLaneBias_mpss;

        if(changeTerm>biasTerm)
            incentive = 1;
    }

    return incentive;
}

double MOBIL::computeRightLaneIncentive()
{
    double incentive = 0;

    PositionInfo info = p_car->getPositionInfo();
    Lane *right = info.lane->getRightNeighbor();
    PerceptionEntry follower = p_perception->getFollower(info.lane);

    bool safe = checkSafetyCondition(right, follower);
    if(safe)
    {
        PerceptionEntry rightLeader = p_perception->getLeader(right);
        double a_c0 = p_idm->computeAcceleration(p_car);
        double a_c1 = p_idm->computeAcceleration(p_car, rightLeader.vehicle, rightLeader.distance_m, right);


        double a_eur = a_c0;
        if(rightLeader.vehicle)
        {
            if(p_car->getSpeed() > rightLeader.speed_mps)
                a_eur = std::min(a_c0, a_c1);
        }

        double a_o0 = 0;
        double a_o1 = 0;
        PerceptionEntry leader = p_perception->getLeader(info.lane);
        if(follower.vehicle)
        {
            a_o0 = p_idm->computeAcceleration(follower.vehicle, p_car, follower.distance_m, right);
            a_o1 = p_idm->computeAcceleration(follower.vehicle, leader.vehicle, follower.distance_m + leader.distance_m, right);
        }

        double changeTerm = a_eur - a_c0 + m_politeness * (a_o1 - a_o0);
        double biasTerm = m_changingThreshold_mpss - m_rightLaneBias_mpss;

        if(changeTerm>biasTerm)
            incentive = 1;
    }

    return incentive;
}

bool MOBIL::checkSafetyCondition(Lane *_lane, const PerceptionEntry &_follower)
{
    bool safe = false;

    if(_lane)
    {
        if(!_follower.vehicle)
            return true;

        double predictedAcceleration = p_idm->computeAcceleration(_follower.vehicle, p_car, _follower.distance_m, _lane);
        safe = (predictedAcceleration >= -m_maxDeceleration_mpss);
    }

    return safe;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/


}
