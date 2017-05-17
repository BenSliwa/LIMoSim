#ifndef LIMOSIM_IDM_H
#define LIMOSIM_IDM_H

#include "followermodel.h"

namespace LIMOSIM
{

class IDM : public FollowerModel
{
public:
    IDM(Car *_car);

    virtual double computeAcceleration(Car *_car);
    virtual double computeAcceleration(Car *_car, Car *_leader, double _distance_m, Lane *_lane);
    virtual double computeAcceleration(double _speed_mps, double _distanceToLeader_m, double _speedDelta_mps, double _desiredSpeed_mps);


    bool drive();

    bool useLeader(Car *_leader);
    bool useTrafficSignal(double _leaderDistance_m, Node *_node, double _distance_m);

    double limitAcceleration(double _acceleration);
    double computeFreeRoadCoefficient(double _speed_mps, double _desiredSpeed_mps);
    double computeBusyRoadCoefficient(double _speed_mps, double _speedDelta_mps, double _distance_m);
    double computeDesiredGap(double _speed_mps, double _speedDelta_mps);
    double computeBreakGap(double _speed_mps, double _speedDelta_mps);

    // property accessors
    void setSafeTimeheadway(double _headway_s);
    void setMaxAcceleration(double _acceleration);
    void setMaxDeceleration(double _deceleration);
    void setAccelerationExponent(double _exponent);
    void setMinDistance(double _distance_m);

private:
    double m_safeTimeHeadway_s;
    double m_maxAcceleration_mpss;
    double m_maxDeceleration_mpss;
    double m_accelerationExponent;
    double m_minDistance_m;
};


}


#endif // LIMOSIM_IDM_H
