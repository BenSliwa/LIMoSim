#include "routinggraph.h"
#include <sstream>
#include <iostream>

namespace LIMOSIM
{

RoutingGraph::RoutingGraph()
{

}

RoutingGraph::~RoutingGraph()
{

}

void RoutingGraph::clear()
{
    std::map<Node*, RoutingNode*>::iterator it;
    for(it=m_nodes.begin(); it!=m_nodes.end(); it++)
    {
        RoutingNode *node = it->second;
        delete node;
    }
}

void RoutingGraph::addConnection(Node *_from, Node *_to, double _cost)
{
    RoutingNode *from = getNode(_from);
    RoutingNode *to = getNode(_to);

    from->addConnection(to, _cost);
}

RoutingNode* RoutingGraph::getNode(Node *_node)
{
    RoutingNode *node = 0;
    if(!m_nodes.count(_node))
    {
        node = new RoutingNode(_node);
        m_nodes[_node] = node;
    }
    else
        node = m_nodes[_node];

    return node;
}

std::vector<RoutingNode*> RoutingGraph::getNodes()
{
    std::vector<RoutingNode*> nodes;

    std::map<Node*, RoutingNode*>::iterator it;
    for(it=m_nodes.begin(); it!=m_nodes.end(); it++)
        nodes.push_back(it->second);

    return nodes;
}

std::string RoutingGraph::toString()
{
    std::stringstream stream;
    std::map<Node*, RoutingNode*>::iterator node;
    for(node=m_nodes.begin(); node!=m_nodes.end(); node++)
    {
        stream << node->first->getName() << ": ";

        std::vector<RoutingNode*> connections = node->second->getConnections();
        for(unsigned int i=0; i<connections.size(); i++)
        {
            stream << connections.at(i)->getNode()->getName();
            if(i<connections.size()-1)
                stream << " ";
        }
        stream << std::endl;
    }

    return stream.str();
}

void RoutingGraph::info()
{
    std::cout << "RoutingGraph::info" << std::endl;
    std::cout << toString() << std::endl;
}

}
