#include "statisticsentry.h"

namespace LIMoSim
{


StatisticsEntry::StatisticsEntry(const std::string _key, double _value, int _type) :
    key(_key),
    value(_value),
    type(_type)
{
    data.push_back(value);
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

double StatisticsEntry::sum()
{
    double sum = 0;
    for(double i : data)
        sum += i;

    return sum;
}

double StatisticsEntry::mean()
{
    return sum() / data.size();
}

}
