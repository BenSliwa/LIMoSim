#include "statisticsmanager.h"

namespace LIMoSim
{

StatisticsManager *statisticsMangerInstance = nullptr;

StatisticsManager::StatisticsManager()
{
    statisticsMangerInstance = this;
}

StatisticsManager::~StatisticsManager()
{
    std::map<std::string, Statistics*>::iterator it;
    for(it=m_statistics.begin(); it!=m_statistics.end(); it++)
    {
        Statistics *statistics = it->second;
        delete statistics;
    }
}

StatisticsManager* StatisticsManager::getInstance()
{
    if(!statisticsMangerInstance)
        statisticsMangerInstance = new StatisticsManager();

    return statisticsMangerInstance;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

Statistics* StatisticsManager::createStatistics(const std::string &_id)
{
    Statistics *statistics = new Statistics(_id);
    m_statistics[_id] = statistics;

    return statistics;
}

Statistics* StatisticsManager::getStatistics(const std::string &_id)
{
    Statistics *statistics = nullptr;
    if(m_statistics.count(_id)>0)
        statistics = m_statistics[_id];

    return statistics;
}

}
