#include "qteventscheduler.h"
#include "core/sim/eventhandler.h"


namespace LIMOSIM
{

QtEventScheduler::QtEventScheduler() :
    QObject(),
    EventScheduler(),
    m_state(SIM_STATE::STOPPED),
    m_simTime_s(0),
    m_simDuration_s(0),
    m_eventCount(0),
    m_timeScaleFactor(10)
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void QtEventScheduler::scheduleEvent(Event *_event)
{
    double timestamp_s = _event->getTimestamp();
    int index = m_eventQueue.size();
    for(unsigned int i=0; i<m_eventQueue.size(); i++)
    {
        Event *event = m_eventQueue.at(i);
        if(event->getTimestamp()>timestamp_s)
        {
            index = i;
            break;
        }
    }

    m_eventQueue.insert(m_eventQueue.begin()+index, _event);
}

void QtEventScheduler::cancelEvent(Event *_event)
{
    int index = -1;
    for(unsigned int i=0; i<m_eventQueue.size(); i++)
    {
        if(m_eventQueue.at(i)==_event)
        {
            index = i;
            break;
        }
    }

    if(index>-1)
        m_eventQueue.erase(m_eventQueue.begin() + index);
}

void QtEventScheduler::deleteEvent(Event *_event)
{
    cancelEvent(_event);
    delete _event;
}

void QtEventScheduler::reset()
{
    m_simTime_s = 0;
    m_simDuration_s = 0;
    m_eventCount = 0;

    stop();

    std::cout << "QtEventScheduler::reset - remaining events" << std::endl;
    for(unsigned int i=0; i<m_eventQueue.size(); i++)
    {
        Event *event = m_eventQueue.at(i);
        std::cout << "Event:\t" << event->getTimestamp() << "\t" << event->getInfo() << std::endl;
    }
}

void QtEventScheduler::start()
{
    if(m_state!=SIM_STATE::RUNNING)
    {
        m_state = SIM_STATE::RUNNING;

        handleNextEvent();
    }
}

void QtEventScheduler::step()
{
    m_state = SIM_STATE::STOPPED;

    handleNextEvent();
}

void QtEventScheduler::stop()
{
    m_state = SIM_STATE::STOPPED;
}

double QtEventScheduler::getScheduledEventCount()
{
    return m_eventQueue.size();
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

double QtEventScheduler::getSimTime()
{
    return m_simTime_s;
}

double QtEventScheduler::getSimDuration()
{
    return m_simDuration_s;
}

int QtEventScheduler::getEventCount()
{
    return m_eventCount;
}

void QtEventScheduler::setTimeScaleFactor(double _factor)
{
    m_timeScaleFactor = _factor;
}



/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void QtEventScheduler::handleNextEvent()
{
    //
    if(m_eventQueue.size()>0)
    {
        Event *event = m_eventQueue.at(0);
        m_eventQueue.pop_front();

        m_eventCount++;
        m_simTime_s = event->getTimestamp();

        event->handle();
    }

    //
    if(m_state==SIM_STATE::RUNNING && m_eventQueue.size()>0) // handle all events with the same timestamp
    {
        double delta_s = m_eventQueue.at(0)->getTimestamp() - m_simTime_s;
        double t = delta_s * 1000;
        t /= m_timeScaleFactor;

        if(delta_s==0)
            handleNextEvent();
        else
        {
            QTimer::singleShot(t, this, SLOT(onUpdated()));
        }
    }


}


void QtEventScheduler::onUpdated()
{
    handleNextEvent();
}



}
