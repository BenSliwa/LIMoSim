#ifndef QTEVENTSCHEDULER_H
#define QTEVENTSCHEDULER_H

#include <QObject>
#include <QTimer>
#include "core/sim/eventscheduler.h"

namespace LIMOSIM
{

class QtEventScheduler : public QObject, public EventScheduler
{
    Q_OBJECT
public:
    QtEventScheduler();

    void scheduleEvent(Event *_event);
    void cancelEvent(Event *_event);
    void deleteEvent(Event *_event);

    // simulation control
    void reset();
    void start();
    void step();
    void stop();

    double getScheduledEventCount();

    // property accessors
    double getSimTime();
    double getSimDuration();
    int getEventCount();

    void setTimeScaleFactor(double _factor);


private:
    void handleNextEvent();

private slots:
    void onUpdated();

private:
    std::deque<Event*> m_eventQueue;
    int m_state;

    double m_simTime_s;
    double m_simDuration_s;
    int m_eventCount;

    double m_timeScaleFactor;

};

}

#endif // QTEVENTSCHEDULER_H
