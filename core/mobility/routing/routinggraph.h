#ifndef LIMOSIM_ROUTINGGRAPH_H
#define LIMOSIM_ROUTINGGRAPH_H

#include <map>
#include <string>
#include <vector>
#include "routingnode.h"

namespace LIMOSIM
{

class RoutingGraph
{
public:
    RoutingGraph();
    ~RoutingGraph();

    void clear();

    void addConnection(Node *_from, Node *_to, double _cost);

    RoutingNode* getNode(Node *_node);
    std::vector<RoutingNode*> getNodes();

    std::string toString();
    void info();


private:
    std::map<Node*, RoutingNode*> m_nodes;

};

}

#endif // LIMOSIM_ROUTINGGRAPH_H
