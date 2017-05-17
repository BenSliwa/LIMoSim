#ifndef STATISTICSENTRY_H
#define STATISTICSENTRY_H

#include <iostream>
#include <vector>

namespace LIMOSIM
{

namespace Stats
{
    enum
    {
        SUM,
        MEAN,
        LAST
    };
}

class StatisticsEntry
{
public:
    StatisticsEntry(const std::string _key = "", double _value = 0, int _type = 0);

    double sum();
    double mean();

public:
    std::string key;
    double value;
    int type;

    std::vector<double> data;
};



}

#endif // STATISTICSENTRY_H
