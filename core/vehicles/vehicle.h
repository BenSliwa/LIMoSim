#ifndef LIMOSIM_VEHICLE_H
#define LIMOSIM_VEHICLE_H

#include <sstream>
#include <iostream>
#include "core/sim/eventhandler.h"

#include "core/location/position.h"
#include "core/location/orientation.h"
#include "core/sim/statistics/statisticsmanager.h"

namespace LIMOSIM
{

class Vehicle : public EventHandler
{
public:
    Vehicle(const std::string &_id, const std::string &_name="");
    ~Vehicle();

    // property accessors
    void setId(const std::string &_id);
    void setName(const std::string &_name);
    void setPosition(const Position &_position);
    void setOrientation(const Orientation &_orientation);


    std::string getId();
    std::string getName();
    Position getPosition();
    Orientation getOrientation();


protected:
    std::string m_id;
    std::string m_name;

    Position m_position;
    Orientation m_orientation;

    double m_updateInterval_s;
    double m_lastUpdate_s;




};

}

#endif // LIMOSIM_VEHICLE_H
