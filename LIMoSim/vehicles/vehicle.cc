#include "vehicle.h"

namespace LIMoSim
{

Vehicle::Vehicle(const std::string &_id, const std::string &_name) : EventHandler(),
    m_id(_id),
    m_name(_name),
    m_lastUpdate_s(0)
{
    if(_name=="")
    {
        std::stringstream stream;
        stream << "V" << _id;

        m_name = stream.str();
    }
}

Vehicle::~Vehicle()
{
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/


/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Vehicle::setId(const std::string &_id)
{
    m_id = _id;
}

void Vehicle::setName(const std::string &_name)
{
    m_name = _name;
}

void Vehicle::setPosition(const Position &_position)
{
    m_position = _position;
}

void Vehicle::setOrientation(const Orientation &_orientation)
{
    m_orientation = _orientation;
}

std::string Vehicle::getId()
{
    return m_id;
}

std::string Vehicle::getName()
{
    return m_name;
}

Position Vehicle::getPosition()
{
    return m_position;
}

Orientation Vehicle::getOrientation()
{
    return m_orientation;
}


}
