#include "routingnode.h"
#include <sstream>
#include <iostream>

namespace LIMoSim
{

RoutingNode::RoutingNode(Node *_node) :
    p_node(_node)
{
}

/****************************************
 *             PUBLIC METHODS           *
 ***************************************/

void RoutingNode::addConnection(RoutingNode *_destination, double _cost)
{
    m_costs[_destination] = _cost;
}

bool RoutingNode::hasConnection(RoutingNode *_destination)
{
    return m_costs.count(_destination);
}

double RoutingNode::getCost(RoutingNode *_destination)
{
    return m_costs[_destination];
}

std::vector<RoutingNode*> RoutingNode::getConnections()
{
    std::vector<RoutingNode*> connections;

    std::map<RoutingNode*, double>::iterator it;
    for(it=m_costs.begin(); it!=m_costs.end(); it++)
        connections.push_back(it->first);

    return connections;
}

/****************************************
 *           PROPERTY ACCESSORS         *
 ***************************************/

Node* RoutingNode::getNode()
{
    return p_node;
}

}
