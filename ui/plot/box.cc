#include "box.h"
#include <math.h>
#include <algorithm>
#include <iostream>

Box::Box()
{
    /*
    m_data.push_back(5);
    m_data.push_back(3);
    m_data.push_back(4);
    m_data.push_back(8);
    m_data.push_back(2);
    m_data.push_back(1);*/
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Box::add(double _value)
{
    m_data.push_back(_value);
}

void Box::update()
{
    double lowerQuartile = getQuartile(25);
    double upperQuartile = getQuartile(75);
    double iqr = upperQuartile - lowerQuartile;
    double lowerWhisker = getLowerWhisker(lowerQuartile, iqr);
    double upperWhisker = getUpperWhisker(upperQuartile, iqr);


    //std::cout << iqr << "\t" << lowerWhisker << "\t" << upperWhisker << std::endl;

}

double Box::getSum()
{
    double sum = 0;
    for(double i : m_data)
        sum += i;

    return sum;
}

double Box::getMean()
{
    return getSum() / m_data.size();
}

double Box::getMedian()
{
    std::sort(m_data.begin(), m_data.end());

    double median = 0;
    int s = m_data.size();
    if(s>0)
    {
        if(s%2==0)
        {
            double m1 = m_data.at(s/2-1);
            double m2 = m_data.at(s/2);

            median = (m1+m2)/2;
        }
        else
            median = m_data.at(m_data.size()/2);
    }

    return median;
}

double Box::getStdDev()
{
    double stdDev = 0;
    double mean = getMean();

    for(double x : m_data)
    {
        stdDev += (x-mean) * (x-mean);
    }

    double f = 1/(double)(m_data.size()-1);
    stdDev = sqrt(f * stdDev);

    return stdDev;
}

double Box::getSEM()
{
    double sem = 0;
    if(m_data.size()>0)
        sem = getStdDev()/sqrt(m_data.size());

    return sem;
}



double Box::getQuartile(double _percent)
{
    std::sort(m_data.begin(), m_data.end());

    double quartile = 0;
    int s = m_data.size();
    for(unsigned int i=0; i<s; i++)
    {
        double x = m_data.at(i);
        double percent = i/(double)s * 100;

        if(percent>_percent)
            break;
        else
            quartile = x;
    }

    return quartile;
}

double Box::getLowerWhisker(double _lowerQuartile, double _iqr)
{
    double whisker = 0;
    double allowedOffset = 1.5 * _iqr;
    for(double x : m_data)
    {
        double offset = _lowerQuartile - x;

        if(offset<allowedOffset)
        {
            whisker = x;
            break;
        }
    }

    return whisker;
}

double Box::getUpperWhisker(double _upperQuartile, double _iqr)
{
    double whisker = 0;
    double allowedOffset = 1.5 * _iqr;

    for(int i=m_data.size()-1; i>-1; i--)
    {
        double x = m_data.at(i);
        double offset = x - _upperQuartile;

        if(offset<allowedOffset)
        {
            whisker = x;
            break;
        }
    }

    return whisker;
}

std::vector<double> Box::getOutliers(double _lowerWhisker, double _upperWhisker)
{
    std::vector<double> outliers;

    for(int i=m_data.size()-1; i>-1; i--)
    {
        double x = m_data.at(i);
        if(x<_lowerWhisker || x>_upperWhisker)
            outliers.push_back(x);
    }

    return outliers;
}
