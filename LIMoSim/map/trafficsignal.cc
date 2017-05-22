#include "trafficsignal.h"
#include "LIMoSim/location/math_.h"
#include <math.h>

namespace LIMoSim
{

TrafficSignal::TrafficSignal(Node *_node) :
    EventHandler(),
    p_node(_node),
    m_state(TRAFFIC_SIGNAL::RED),
    m_green_s(10),
    m_yellow_s(3),
    m_red_s(14),
    m_redYellow_s(1),
    m_direction(WAY_DIRECTION::BIDIRECTIONAL),
    m_wayAngle(0)
{
    m_updateTimer = new Event(0, this, "TrafficSignal::updateTimer");
    scheduleEvent(m_updateTimer);
}

TrafficSignal::~TrafficSignal()
{
    deleteEvent(m_updateTimer);
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


void TrafficSignal::initialize()
{

}

void TrafficSignal::finish()
{

}

void TrafficSignal::setStateWithAngle(double _reference)
{
    double delta = Math::getMinimumAngleDifference(m_wayAngle, _reference);
    int index = fabs(delta)/45;

    if(index%2!=0)
        setState(TRAFFIC_SIGNAL::RED);
    else
        setState(TRAFFIC_SIGNAL::GREEN);

   // cancelEvent(m_updateTimer);
   // scheduleEvent(m_updateTimer, getStateDuration(m_state));
}

double TrafficSignal::getStateDuration(int _state)
{
    double duration_s;
    switch(_state)
    {
        case TRAFFIC_SIGNAL::GREEN: duration_s=m_green_s; break;
        case TRAFFIC_SIGNAL::YELLOW: duration_s=m_yellow_s; break;
        case TRAFFIC_SIGNAL::RED: duration_s=m_red_s; break;
        case TRAFFIC_SIGNAL::RED_YELLOW: duration_s=m_redYellow_s; break;
    }

    return duration_s;
}

int TrafficSignal::getNextState(int _state)
{
    int state;
    switch(_state)
    {
        case TRAFFIC_SIGNAL::GREEN: state=TRAFFIC_SIGNAL::YELLOW; break;
        case TRAFFIC_SIGNAL::YELLOW: state=TRAFFIC_SIGNAL::RED; break;
        case TRAFFIC_SIGNAL::RED: state=TRAFFIC_SIGNAL::RED_YELLOW; break;
        case TRAFFIC_SIGNAL::RED_YELLOW: state=TRAFFIC_SIGNAL::GREEN; break;
    }

    return state;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void TrafficSignal::setState(int _state)
{
    m_state = _state;
}

void TrafficSignal::setDirection(int _direction)
{
    m_direction = _direction;
}

void TrafficSignal::setWayAngle(double _angle)
{
    m_wayAngle = _angle;
}

Node* TrafficSignal::getNode()
{
    return p_node;
}

int TrafficSignal::getState()
{
    return m_state;
}

int TrafficSignal::getDirection()
{
    return m_direction;
}

double TrafficSignal::getWayAngle()
{
    return m_wayAngle;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void TrafficSignal::handleEvent(Event *_event)
{
    if(_event==m_updateTimer)
    {
        if(m_updateTimer->getTimestamp()>0)
            setState(getNextState(m_state));
        scheduleEvent(m_updateTimer, getStateDuration(m_state));
    }
}


}
