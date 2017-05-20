#include "randomdestination.h"
#include "LIMoSim/sim/rng.h"
#include "LIMoSim/mobility/routing/dijkstra.h"

namespace LIMoSim
{

RandomDestination::RandomDestination(Car *_car) :
    FollowPath(_car),
    m_pathStartTime_s(-1)
{
    m_type = "RandomDestination";
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/


void RandomDestination::updatePath(PositionInfo *_info, const MobilityUpdate &_update)
{
    int pathSize = _info->path.size();

    if(pathSize==0)
    {

        // determine a random destination that is not equal to the reached node
        Node *start = _update.nextNode;
        Node *destination = getRandomDestination();
        while(destination==_update.lastNode || destination==_update.nextNode)
            destination = getRandomDestination();


        std::cout << "determing a new path from " << start->getName() << "\tto:" << destination->getName() << std::endl;

        std::vector<Node*> path = computeRoutingPath(start, destination, _update.lastNode);
        // determine a routing path from the reached node to the destintion

        if(path.size()>1)
        {

        }
        else
        {
            path = computeRoutingPath(start, destination, 0);
        }

        path.erase(path.begin()); // remove the first node from the path as it is equal to the current 1-hop destination
        _info->path = path;
    }
}


std::pair<Node*,int> RandomDestination::getNextHop(Segment *_segment, Node *_node, Node *_previous)
{
    std::cout << "RandomDestination::getNextHop " << _node->getName() << std::endl;

    std::pair<Node*,int> nextHop;

    return nextHop;
}

std::vector<Node*> RandomDestination::computeRoutingPath(Node *_from, Node *_to, Node *_without)
{
    Map *map = Map::getInstance();
    RoutingGraph graph = map->createGraph(_from, _to, _without);
    Dijkstra dijkstra;
    dijkstra.setGraph(&graph);
    RoutingPath routingPath = dijkstra.computeShortestPath(graph.getNode(_from), graph.getNode(_to));

    std::vector<Node*> path = map->convertRoutingPath(routingPath, _without);
    graph.clear();


    return path;
}

/*
void RandomDestination::handleNodeReached(Node *_node)
{
    std::cout << "RandomDestination::handleNodeReached " << _node->getName() << std::endl;

    PositionInfo info = p_car->getPositionInfo();
    if(info.path.size()>0)
    {

    }
    else
    {
        //std::cout << "new path" << std::endl;

        // compute travel time
        double simTime_s = Simulation::getInstance()->getEventScheduler()->getSimTime();
        if(m_pathStartTime_s>-1)
        {
            double travelTime_s = simTime_s - m_pathStartTime_s;
        }
        m_pathStartTime_s = simTime_s;


        // determine a random destination that is not equal to the reached node
        Node *destination = getRandomDestination();
        while(destination==_node)
            destination = getRandomDestination();



        // determine a routing path from the reached node to the destintion
        Map *map = Map::getInstance();
        RoutingGraph graph = map->createGraph(_node, destination);
        Dijkstra dijkstra;
        dijkstra.setGraph(&graph);
        RoutingPath routingPath = dijkstra.computeShortestPath(graph.getNode(_node), graph.getNode(destination));



        std::vector<Node*> path = map->convertRoutingPath(routingPath);
        graph.clear();
        path.erase(path.begin());
        info.path = path;


    }

    info.currentNode = info.path.at(0);
    info.path.erase(info.path.begin());

    /*  TODO: failure handling and rerouting if nodes have been missed
     *
     *
     *
     *
     */

/*
    // determine the lane for the next node
    std::vector<Segment*>segments = _node->getSegments();
    for(unsigned int i=0; i<segments.size(); i++)
    {
        Segment *segment = segments.at(i);
        Node *start = segment->getStartGate()->node;
        Node *end = segment->getEndGate()->node;

        if(start==_node && end==info.currentNode)
        {
            int type = _node->checkSegmentDirection(segment);
            if(type!=WAY_DIRECTION::BACKWARD)
                info.lane = segment->getLanes().at(1);
        }
        else if(start==info.currentNode && end==_node)
        {
            int type = _node->checkSegmentDirection(segment);
            if(type!=WAY_DIRECTION::BACKWARD)
                info.lane = segment->getLanes().at(0);
        }
    }


    p_car->setPositionInfo(info);

    //std::cout << "reached: " << _node->getName() << "\tnext: " << info.nextNode->getName() << std::endl;
    //info.info();

}
*/

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

Node* RandomDestination::getRandomDestination()
{
    Map *map = Map::getInstance();
    std::map<std::string,Node*> nodes = map->getNodes();
    std::vector<Node*> nodesList;

    std::map<std::string,Node*>::iterator it;
    for(it=nodes.begin(); it!=nodes.end(); it++)
    {
        nodesList.push_back(it->second);
    }

    int index = RNG::intUniform(0, nodesList.size()-1);
    return nodesList.at(index);
}

Lane* RandomDestination::getLane(const DestinationEntry &_entry)
{
    Node *nextNode = _entry.neighbor;
    Segment *segment = _entry.segment;
    Lane *lane = 0;

    if(nextNode==segment->getEndGate()->node) // with segment direction
        lane = segment->getLanes().at(1);
    else
        lane = segment->getLanes().at(0);

    return lane;
}




/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

}
