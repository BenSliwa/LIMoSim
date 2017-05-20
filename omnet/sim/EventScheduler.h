//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_EVENTSCHEDULER_H_
#define __INET_EVENTSCHEDULER_H_

#include <omnetpp.h>
#include "LIMoSim/sim/event.h"
#include "LIMoSim/sim/eventscheduler.h"

using namespace omnetpp;

namespace inet
{

class EventScheduler : public cSimpleModule, public LIMoSim::EventScheduler
{
public:
    EventScheduler();
    ~EventScheduler();

    static EventScheduler* getInstance();
    void handleStart();

    void scheduleEvent(LIMoSim::Event *_event);
    void cancelEvent(LIMoSim::Event *_event);
    void deleteEvent(LIMoSim::Event *_event);

    cMessage* getMessageForEvent(LIMoSim::Event *_event);

protected:
    virtual void handleMessage(cMessage *_message);

private:
    std::map<cMessage*, LIMoSim::Event*> m_events;

};

} //namespace

#endif
