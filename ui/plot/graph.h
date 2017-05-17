#ifndef LIMOSIM_GRAPH_H
#define LIMOSIM_GRAPH_H

#include <QList>
#include <QPen>
#include "linestyle.h"

namespace LIMoSim
{

struct Point
{
    double x;
    double y;
};

class Graph
{
public:
    Graph();

    void add(const Point &_point);
    void adjustToRange(double _max, double _range);

    Point getXRange();

    // property accessors
    void setId(const QString &_id);
    void setStyle(const LineStyle &_style);

    QString getId();
    LineStyle getStyle();
    QList<Point>& getPoints();

private:
    QString m_id;
    LineStyle m_style;

    QList<Point> m_points;



};

}


#endif // LIMOSIM_GRAPH_H
