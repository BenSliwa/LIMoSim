#ifndef LIMOSIM_STATISTICS_H
#define LIMOSIM_STATISTICS_H

#include "LIMoSim/sim/eventhandler.h"
#include "LIMoSim/settings/filehandler.h"
#include "statisticsentry.h"

namespace LIMoSim
{

class Statistics : public EventHandler
{
public:
    Statistics(const std::string &_id);
    ~Statistics();

    void handleEvent(Event *_event);

    void addKey(const std::string &_key);
    void add(const StatisticsEntry &_entry);

protected:
    Event *m_updateTimer;


    double m_updateInterval;

    std::string m_id;
    std::string m_log;

    std::vector<std::string> m_keys;
    std::map<std::string, StatisticsEntry> m_data;
};

}

#endif // LIMOSIM_STATISTICS_H
