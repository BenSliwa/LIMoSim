#include "graph.h"
#include <limits>
#include <QDebug>

namespace LIMoSim
{

Graph::Graph()
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Graph::add(const Point &_point)
{
    m_points << _point;
}

void Graph::adjustToRange(double _max, double _range)
{
    if(m_points.size()>0)
    {
        for(int i=0; i<m_points.size();)
        {
            double x = m_points.at(i).x;
            if(_max-x>_range)
                m_points.removeAt(i);
            else
                ++i;
        }
    }
}

Point Graph::getXRange()
{
    Point range{std::numeric_limits<double>::max(), std::numeric_limits<double>::min()};

    for(int i=0; i<m_points.size(); i++)
    {
        double x = m_points.at(i).x;
        if(x<range.x)
            range.x = x;
        if(x>range.y)
            range.y = x;
    }

    return range;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Graph::setId(const QString &_id)
{
    m_id = _id;
}

void Graph::setStyle(const LineStyle &_style)
{
    m_style = _style;
}

QString Graph::getId()
{
    return m_id;
}

LineStyle Graph::getStyle()
{
    return m_style;
}

QList<Point>& Graph::getPoints()
{
    return m_points;
}



}
