#ifndef LIMOSIM_EVENT_H
#define LIMOSIM_EVENT_H

#include <string>

namespace LIMoSim
{
class EventHandler;

class Event
{
public:
    Event(double _timestamp_s, EventHandler *_handler, const std::string &_info = "");

    void handle();

    // property accessors
    void setTimestamp(double _timestamp_s);
    void setInfo(const std::string &_info);

    double getTimestamp();
    EventHandler* getHandler();
    std::string getInfo();

private:
    double m_timestamp_s;
    EventHandler *p_handler;
    std::string m_info;
};

}

#endif // LIMOSIM_EVENT_H
