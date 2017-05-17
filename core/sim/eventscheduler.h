#ifndef LIMOSIM_EVENTSCHEDULER_H
#define LIMOSIM_EVENTSCHEDULER_H

#include <deque>
#include "event.h"
#include "simulationdefinitions.h"


namespace LIMOSIM
{

class EventScheduler
{
public:
    EventScheduler();

    // events
    virtual void scheduleEvent(Event *_event) = 0;
    virtual void cancelEvent(Event *_event) = 0;
    virtual void deleteEvent(Event *_event) = 0;
};


}

#endif // LIMOSIM_EVENTSCHEDULER_H
