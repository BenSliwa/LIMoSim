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

#include "EventScheduler.h"

namespace inet {

EventScheduler *eventSchedulerInstance = 0;

Define_Module(EventScheduler);

EventScheduler::EventScheduler()
{

}

EventScheduler::~EventScheduler()
{
    std::map<cMessage*, LIMoSim::Event*>::iterator it;
    for(it=m_events.begin(); it!=m_events.end(); it++)
    {
        deleteEvent(it->second);
    }
}

EventScheduler* EventScheduler::getInstance()
{
    if(!eventSchedulerInstance)
    {
        cModule *network = cSimulation::getActiveSimulation()->getSystemModule();
        cModuleType* nodeType = cModuleType::get("inet.LIMoSim.omnet.sim.EventScheduler");

        cModule *module = nodeType->create("EventScheduler", network, 0, 0);
        module->finalizeParameters();
        //mod->getDisplayString().parse(displayString.c_str());
        module->buildInside();

        eventSchedulerInstance = dynamic_cast<EventScheduler*>(module);
        eventSchedulerInstance->handleStart();
    }

    return eventSchedulerInstance;
}

void EventScheduler::handleStart()
{
    Enter_Method("handleStart");

    cMessage *timer = new cMessage();
    scheduleAt(simTime(), timer);
}

void EventScheduler::scheduleEvent(LIMoSim::Event *_event)
{
    Enter_Method("scheduleEvent");

    cMessage *event = new cMessage();
    m_events[event] = _event;

    scheduleAt(_event->getTimestamp(), event);
}

void EventScheduler::cancelEvent(LIMoSim::Event *_event)
{
    cMessage *message = getMessageForEvent(_event);
    if(message)
    {
        cancelAndDelete(message);

        m_events.erase(message);
        delete _event;
    }

}

void EventScheduler::deleteEvent(LIMoSim::Event *_event)
{
    cancelEvent(_event);
}

cMessage* EventScheduler::getMessageForEvent(LIMoSim::Event *_event)
{
    cMessage *message = 0;
    std::map<cMessage*, LIMoSim::Event*>::iterator it;
    for(it=m_events.begin(); it!=m_events.end(); it++)
    {
        if(it->second==_event)
            message = it->first;
    }

    return message;
}

void EventScheduler::handleMessage(cMessage *_message)
{
    if(_message->isSelfMessage())
    {
        if(m_events.count(_message))
        {
            LIMoSim::Event *event = m_events[_message];
            event->handle();


            //
            m_events.erase(_message);
            delete _message;
        }
    }
}

} //namespace
