#include "routingpath.h"
#include <iostream>

namespace LIMoSim
{

RoutingPath::RoutingPath()
{

}

/****************************************
 *             PUBLIC METHODS           *
 ***************************************/

void RoutingPath::append(RoutingNode *_node)
{
    m_nodes.insert(m_nodes.end(), _node);
}

void RoutingPath::prepend(RoutingNode *_node)
{
    m_nodes.insert(m_nodes.begin(), _node);
}

RoutingNode* RoutingPath::at(int _index) const
{
    RoutingNode *node = 0;
    if(_index>-1 && _index<m_nodes.size())
        node = m_nodes.at(_index);

    return node;
}

int RoutingPath::size() const
{
    return m_nodes.size();
}

double RoutingPath::cost() const
{
    double cost = 0;

    for(unsigned int i=0; i<m_nodes.size(); i++)
    {
        RoutingNode *node = m_nodes.at(i);
        if(i>0)
        {
            RoutingNode *previous = m_nodes.at(i-1);
            cost += previous->getCost(node);
        }
    }

    return cost;
}

std::string RoutingPath::toString()
{
    std::stringstream stream;

    stream << "ROUTE: " << cost() << "\t";

    for(unsigned int i=0; i<m_nodes.size(); i++)
    {
        RoutingNode *node = m_nodes.at(i);
        if(i>0)
        {
            RoutingNode *previous = m_nodes.at(i-1);
            double cost = previous->getCost(node);

            stream << cost << " -> ";
        }

        stream << node->getNode()->getName();
        if(i<m_nodes.size()-1)
            stream << " -> ";
    }

    return stream.str();
}

void RoutingPath::info()
{
    std::cout << toString() << std::endl;
}


}
