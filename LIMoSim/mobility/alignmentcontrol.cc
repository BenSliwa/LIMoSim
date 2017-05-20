#include "alignmentcontrol.h"
#include <algorithm>
#include <math.h>
#include "LIMoSim/vehicles/car.h"

namespace LIMoSim
{

AlignmentControl::AlignmentControl() :
    m_state(ALIGN::IDLE),
    m_increment_m(0.01)
{

}


double AlignmentControl::updateLateralPosition(Lane *_lane, double _alignment_m, Car *_car)
{
    double alignment_m = _alignment_m;
    double increment_m = 0;

    if(m_state==ALIGN::IDLE)
    {
        int sign = (_alignment_m<0) ? 1 : -1;
        increment_m = sign * std::min(m_increment_m, fabs(_alignment_m));          // TODO: increment as a function of the current speed
    }
    else
    {

        if(m_state==ALIGN::CHANGE_TO_LEFT)
            increment_m = -m_increment_m;
        else if(m_state==ALIGN::CHANGE_TO_RIGHT)
            increment_m = m_increment_m;

        double width_m = 3.5;
        if(fabs(_alignment_m)>width_m/2)
        {
            // assign the lane to the new lane


            Lane *neighbor = 0;
            if(m_state==ALIGN::CHANGE_TO_LEFT)
                neighbor = _lane->getLeftNeighbor();
            else if(m_state==ALIGN::CHANGE_TO_RIGHT)
                neighbor = _lane->getRightNeighbor();


            if(neighbor)
            {
                _car->assignLane(neighbor);

                if(m_state==ALIGN::CHANGE_TO_LEFT)
                    alignment_m += neighbor->getWidth();
                else if(m_state==ALIGN::CHANGE_TO_RIGHT)
                    alignment_m -= neighbor->getWidth();
            }



            //
            m_state = ALIGN::IDLE;

        }

    }

    return alignment_m + increment_m;
}

void AlignmentControl::changeLane(int _type, double _updateInterval_s)
{
    if(m_state==ALIGN::IDLE)
    {
        double width_m = 3.5;
        double duration_s = 2;
        int steps = duration_s/_updateInterval_s;

        m_state = _type;
    }
}

int AlignmentControl::getState()
{
    return m_state;
}

}
