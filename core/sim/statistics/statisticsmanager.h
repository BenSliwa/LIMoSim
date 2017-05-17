#ifndef STATISTICSMANAGER_H
#define STATISTICSMANAGER_H

#include "statistics.h"
#include <map>

namespace LIMoSim
{

class StatisticsManager
{
public:
    StatisticsManager();
    ~StatisticsManager();

    static StatisticsManager* getInstance();


    Statistics* createStatistics(const std::string &_id);
    Statistics* getStatistics(const std::string &_id);


private:
    std::map<std::string, Statistics*> m_statistics;

};

}


#endif // STATISTICSMANAGER_H
