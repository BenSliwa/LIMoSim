#ifndef LIMOSIM_EVENTHANDLER_H
#define LIMOSIM_EVENTHANDLER_H

#include "event.h"
#include "simulation.h"

namespace LIMOSIM
{

class EventHandler
{
public:
    EventHandler();

    void scheduleEvent(Event *_event, double _timeIncrement_s = 0);

    void cancelEvent(Event *_event);
    void deleteEvent(Event *_event);

    virtual void handleEvent(Event *_event) = 0;

protected:
    Simulation *p_simulation;

};

}


#endif // LIMOSIM_EVENTHANDLER_H
