#ifndef ROUTINGPATH_H
#define ROUTINGPATH_H

#include <vector>
#include <sstream>
#include "routingnode.h"

namespace LIMOSIM
{

class RoutingPath
{
public:
    RoutingPath();

    void append(RoutingNode *_node);
    void prepend(RoutingNode *_node);
    RoutingNode* at(int _index) const;
    int size() const;

    double cost() const;

    std::string toString();
    void info();

private:
    std::vector<RoutingNode*> m_nodes;

};

}

#endif // ROUTINGPATH_H
