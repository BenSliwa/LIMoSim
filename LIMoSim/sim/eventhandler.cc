#include "eventhandler.h"


namespace LIMoSim
{

EventHandler::EventHandler() :
    p_simulation(Simulation::getInstance())
{
    p_simulation->registerEventHandler(this);
}

EventHandler::~EventHandler()
{
    p_simulation->deregisterEventHandler(this);
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void EventHandler::initialize()
{

}

void EventHandler::finish()
{

}

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
