#include "trip.h"

namespace LIMoSim
{

Trip::Trip(Car *_car) :
    FollowPath(_car)
{
    m_type = "Trip";
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Trip::addTripEntry(Node *_node)
{
    m_trip.push_back(_node);
}

void Trip::updatePath(PositionInfo *_info, const MobilityUpdate &_update)
{
    if(m_path.size()>0)
    {
        m_path.erase(m_path.begin());           // !!!
        _info->path.insert(_info->path.end(), m_path.begin(), m_path.end());
        m_path.clear();
    }
    else
    {

        Node *destination = m_trip.at(0);
        m_trip.erase(m_trip.begin());
        m_trip.push_back(destination); // loop

        m_path = computePath(_update.lastNode, destination, _update.lastNode);
        m_path.erase(m_path.begin());

        _info->path.insert(_info->path.end(), m_path.begin(), m_path.end());
        m_path.clear();
    }


}

std::pair<Node*,int> Trip::getNextHop(Segment *_segment, Node *_node, Node *_previous)
{
    std::pair<Node*,int> nextHop;

    Node *destination = m_trip.at(0);
    m_trip.erase(m_trip.begin());
    m_trip.push_back(destination); // loop

    m_path = computePath(_node, destination, _previous);

    nextHop.first = m_path.at(0);
    nextHop.second = TURN::THROUGH; // TODO: determine the real turn type

    return nextHop;
}

std::vector<Node*> Trip::computePath(Node *_from, Node *_to, Node *_without)
{
    Map *map = Map::getInstance();

    RoutingGraph graph = map->createGraph(_from, _to, _without);
    Dijkstra dijkstra;
    dijkstra.setGraph(&graph);
    RoutingPath routingPath = dijkstra.computeShortestPath(graph.getNode(_from), graph.getNode(_to));
    std::vector<Node*> path = map->convertRoutingPath(routingPath, _without);
    graph.clear();

    path.erase(path.begin()); // remove the first node of the path as it is the reached node

    /*

    std::cout << "******************** PATH ********************" << std::endl;
    for(unsigned int i=0; i<path.size(); i++)
    {
        std::cout << path.at(i)->getId() << std::endl;
    }
    std::cout << "******************** PATH ********************" << std::endl;*/


    return path;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


}
