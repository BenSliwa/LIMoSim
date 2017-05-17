#ifndef BOX_H
#define BOX_H

#include "vector"

class Box
{
public:
    Box();

    void add(double _value);
    void update();

    double getSum();
    double getMean();
    double getMedian();
    double getStdDev();
    double getSEM();   
    double getQuartile(double _percent);
    double getLowerWhisker(double _lowerQuartile, double _iqr);
    double getUpperWhisker(double _upperQuartile, double _iqr);
    std::vector<double> getOutliers(double _lowerWhisker, double _upperWhisker);

private:
    std::vector<double> m_data;

};

#endif // BOX_H
