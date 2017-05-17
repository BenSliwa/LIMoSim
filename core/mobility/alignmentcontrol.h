#ifndef ALIGNMENTCONTROL_H
#define ALIGNMENTCONTROL_H

#include "core/map/way.h"
#include "core/map/segment.h"
#include "core/map/lane.h"

/* IDLE: keep the alignment as low as possible
 * LANE CHANGE: move until the point of no return is reached, perform the change and go to IDLE
 *
 *
 *
 */


namespace LIMoSim
{

class Car;

namespace ALIGN
{
    enum
    {
        IDLE,
        CHANGE_TO_RIGHT,
        CHANGE_TO_LEFT
    };
}

class AlignmentControl
{
public:
    AlignmentControl();

    double updateLateralPosition(Lane *_lane, double _alignment_m, Car *_car);

    void changeLane(int _type, double _updateInterval_s);

    // property accessors
    int getState();

private:
    int m_state;
    double m_increment_m;
};

}

#endif // ALIGNMENTCONTROL_H
