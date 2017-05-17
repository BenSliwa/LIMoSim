#include "eventhandler.h"


namespace LIMOSIM
{

EventHandler::EventHandler() :
    p_simulation(Simulation::getInstance())
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void EventHandler::scheduleEvent(Event *_event, double _timeIncrement_s)
{
    _event->setTimestamp(_event->getTimestamp() + _timeIncrement_s);
    p_simulation->getEventScheduler()->scheduleEvent(_event);
}

void EventHandler::cancelEvent(Event *_event)
{
    p_simulation->getEventScheduler()->cancelEvent(_event);
}

void EventHandler::deleteEvent(Event *_event)
{
    p_simulation->getEventScheduler()->deleteEvent(_event);
}

}
