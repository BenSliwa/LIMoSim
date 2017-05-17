#ifndef LIMOSIM_DIJKSTRA_H
#define LIMOSIM_DIJKSTRA_H

#include <deque>
#include "routinggraph.h"
#include "routingpath.h"


/*
 * TODO: remove the node reference from routingnode,
 * better uses a map: routingnode->node in the caller
 *
 *
 */

namespace LIMOSIM
{

class Dijkstra
{
public:
    Dijkstra();

    RoutingPath computeShortestPath(RoutingNode *_start, RoutingNode *_destination);

    // property accessors
    void setGraph(RoutingGraph *_graph);

private:
    void init(RoutingNode *_start);
    void updateDistance(RoutingNode *_u, RoutingNode *_v);
    RoutingNode* getNearestNode();
    void removeNode(RoutingNode *_node);
    int getNodeIndex(RoutingNode *_node);

private:
    RoutingGraph *p_graph;

    std::vector<RoutingNode*> m_q;
    std::map<RoutingNode*,double> m_distances;
    std::map<RoutingNode*,RoutingNode*> m_predecessors;
};

}


#endif // LIMOSIM_DIJKSTRA_H
