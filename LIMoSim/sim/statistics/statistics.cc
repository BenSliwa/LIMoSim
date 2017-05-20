#include "statistics.h"

namespace LIMoSim
{

Statistics::Statistics(const std::string &_id) :
    EventHandler(),
    m_updateInterval(1),
    m_id(_id)
{
    m_updateTimer = new Event(0, this, "Statistics::updateTimer");
    scheduleEvent(m_updateTimer, m_updateInterval);

    Event *writeTimer = new Event(100, this,  "Statistics::writeTimer");
    scheduleEvent(writeTimer);
}

Statistics::~Statistics()
{
    deleteEvent(m_updateTimer);
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Statistics::initialize()
{
    std::cout << "Statistics::initialize" << std::endl;
}

void Statistics::finish()
{
    std::cout << "Statistics::finish" << std::endl;
}


void Statistics::handleEvent(Event *_event)
{
    if(_event==m_updateTimer)
    {
        std::stringstream stream;
        stream << m_updateTimer->getTimestamp() << ",";

        //

        for(unsigned int i=0; i<m_keys.size(); i++)
        {
            std::string key = m_keys.at(i);
            StatisticsEntry entry = m_data[key];

            switch(entry.type)
            {
                case Stats::SUM:
                    stream << entry.value << ",";
                break;

                case Stats::LAST:
                    stream << entry.value << ",";
                break;


                case Stats::MEAN:
                    stream << entry.mean() << ",";
                    entry.data.clear();
                break;

            }

            m_data[key] = entry;
        }

        m_log += stream.str() + "\n";


        scheduleEvent(m_updateTimer, m_updateInterval);
    }
    else
    {
        FileHandler fileHandler;
        fileHandler.write(m_log, m_id + ".txt");
    }
}

void Statistics::addKey(const std::string &_key)
{
    int index = -1;
    for(unsigned int i=0; i<m_keys.size(); i++)
    {
        if(m_keys.at(i)==_key)
            index = i;
    }

    if(index==-1)
        m_keys.push_back(_key);
}

void Statistics::add(const StatisticsEntry &_entry)
{
    std::string key = _entry.key;
    if(m_data.count(key)>0)
    {
        switch(_entry.type)
        {
            m_data[key].data.clear();
            case Stats::SUM: m_data[key].value += _entry.value; break;
            case Stats::MEAN: m_data[key].data.push_back(_entry.value); break;
            case Stats::LAST: m_data[key].value = _entry.value; break;
        }
    }
    else
        m_data[key] = _entry;
}



}
