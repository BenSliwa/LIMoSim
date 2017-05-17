#include "event.h"
#include "eventhandler.h"
#include <typeinfo>

namespace LIMOSIM
{

Event::Event(double _timestamp_s, EventHandler *_handler, const std::string &_info) :
    m_timestamp_s(_timestamp_s),
    p_handler(_handler),
    m_info(_info)
{

}

void Event::handle()
{
    p_handler->handleEvent(this);
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Event::setTimestamp(double _timestamp_s)
{
    m_timestamp_s = _timestamp_s;
}

double Event::getTimestamp()
{
    return m_timestamp_s;
}

EventHandler* Event::getHandler()
{
    return p_handler;
}

std::string Event::getInfo()
{
    return m_info;
}

}
