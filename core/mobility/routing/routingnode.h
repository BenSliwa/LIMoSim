#ifndef LIMOSIM_ROUTINGNODE_H
#define LIMOSIM_ROUTINGNODE_H

#include <map>
#include <string>
#include <vector>
#include "core/map/node.h"

namespace LIMOSIM
{

class RoutingNode
{
public:
    RoutingNode(Node *_node = 0);

    void addConnection(RoutingNode *_destination, double _cost);
    bool hasConnection(RoutingNode *_destination);
    double getCost(RoutingNode *_destination);
    std::vector<RoutingNode*> getConnections();

    // property accessors
    Node* getNode();


public:
    Node *p_node;
    std::map<RoutingNode*, double> m_costs;
};

}

#endif // LIMOSIM_ROUTINGNODE_H
