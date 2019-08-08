#include "map.h"
#include "node.h"
#include "trafficsignal.h"
#include "way.h"
#include "LIMoSim/mobility/routing/routinggraph.h"
#include "LIMoSim/mobility/routing/routingpath.h"
#include "LIMoSim/vehicles/car.h"
#include "LIMoSim/sim/rng.h"
#include "LIMoSim/location/wgs84.h"

namespace LIMoSim
{

Map *mapInstance = nullptr;

Map::Map() :
    m_nodeId(0),
    m_wayId(0)
{
  //  mapInstance = this;

}

Map::~Map()
{
    clear();
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

Map* Map::getInstance()
{
    if(!mapInstance)
        mapInstance = new Map();

    return mapInstance;
}

void Map::clear()
{
    //
    std::map<std::string,Node*>::iterator nodes;
    for(nodes=m_nodes.begin(); nodes!=m_nodes.end(); nodes++)
        delete nodes->second;
    m_nodes.clear();

    //
    std::map<std::string,Way*>::iterator ways;
    for(ways=m_ways.begin(); ways!=m_ways.end(); ways++)
        delete ways->second;
    m_ways.clear();

    //
    std::map<std::string,Car*>::iterator cars;
    for(cars=m_cars.begin(); cars!=m_cars.end(); cars++)
        delete cars->second;
    m_cars.clear();
}

MapBounds Map::computeBounds()
{
    MapBounds bounds;

    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::min();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::min();

    std::map<std::string, Node*>::iterator it;
    for(it=m_nodes.begin(); it!=m_nodes.end(); it++)
    {
        Position position = it->second->getPosition();
        double x = position.x;
        double y = position.y;

        if(x<minX)
            minX = x;
        if(x>maxX)
            maxX = x;
        if(y<minY)
            minY = y;
        if(y>maxY)
            maxY = y;
    }

    bounds.origin = Position(minX, minY);
    bounds.width = maxX - minX;
    bounds.height = maxY - minY;
    bounds.center = bounds.origin + Position(bounds.width/2, bounds.height/2);

    return bounds;
}

Node* Map::createNode(const Position &_position, const std::string &_id)
{
    //
    std::string id = _id;
    if(id=="")
    {
        int i = 0;
        id = int2string(i);
        while(getNode(id))
        {
            i++;
            id = int2string(i);
        }
    }

    //
    Node *node = getNode(id);
    if(!node)
    {
        node = new Node(_position, id, NODE_TYPE::NODE);
        m_nodes[id] = node;
    }

    return node;
}

void Map::removeNode(Node *_node)
{
    // remove the node from all its ways
    std::vector<Segment*> segments = _node->getSegments();
    for(unsigned int i=0; i<segments.size(); i++)
    {
        Segment *segment = segments.at(i);
        Way *way = segment->getWay();
        Node *neighbor = segment->getOtherNode(_node);

        //neighbor->removeNeighbor(_node);
        way->removeNode(_node);

        // TODO: better only update the neighbor segments
        way->linkLanes();
    }


    std::vector<DestinationEntry> destinations = _node->getOutgoingConnections();

    // remove the object
    m_nodes.erase(_node->getId());
    delete _node;

    // update all nodes that had a path to the node
    updateAllNodesWithDestination(_node);

    // update all nodes that had the same destinations
    for(unsigned int i=0; i<destinations.size(); i++)
    {
        DestinationEntry entry = destinations.at(i);
        updateAllNodesWithDestination(entry.destination);
    }
}

void Map::linkSegments()
{
    std::map<std::string,Node*>::iterator nodes;
    for(nodes=m_nodes.begin(); nodes!=m_nodes.end(); nodes++)
    {
        Node *node = nodes->second;
        node->linkSegments();
    }
}

void Map::updateAllNodesWithDestination(Node *_destination)
{
    std::map<std::string,Node*>::iterator nodes;
    for(nodes=m_nodes.begin(); nodes!=m_nodes.end(); nodes++)
    {
        Node *node = nodes->second;
        if(node->knowsDestination(_destination))
            node->computeDestinations();
    }
}

Way* Map::createWay(const std::string &_id)
{
    std::string id = _id;
    if(id=="")
    {
        int i = 0;
        id = int2string(i);
        while(getWay(id))
        {
            i++;
            id = int2string(i);
        }
    }

    //
    Way *way = getWay(id);
    if(!way)
    {
        way = new Way(id, WAY_TYPE::WAY, "");
        m_ways[id] = way;
    }

    return way;
}


Car* Map::createCar(const std::string &_id)
{
    std::string id = _id;
    if(id=="")
    {
        int i = 0;
        id = int2string(i);
        while(getCar(id))
        {
            i++;
            id = int2string(i);
        }
    }

    //
    Car *car = getCar(id);
    if(!car)
    {
        car = new Car(id);
        m_cars[id] = car;
    }

    return car;
}

void Map::setRandomPosition(Car *_car)
{
    PositionInfo info;
    Way *way = getRandomWay();
    Segment *segment = nullptr;
    while(!segment)
    {
        way = getRandomWay();
        segment = getRandomSegment(way);
    }


    info.lane = getRandomLane(segment);

    info.currentNode = info.lane->getEndEndpoint()->node;
    info.laneOffset_m = RNG::intUniform(0, info.lane->getLength());

    _car->setPositionInfo(info);
    _car->setPosition(_car->computeLanePosition(info.laneOffset_m, info.alignment_m, true));
}

Way* Map::getRandomWay()
{
    std::vector<Way*> ways;
    std::map<std::string,Way*>::iterator it;
    for(it=m_ways.begin(); it!=m_ways.end(); it++)
        ways.push_back(it->second);

    return ways.at(RNG::intUniform(0, ways.size()-1));
}

Segment* Map::getRandomSegment(Way *_way)
{
    int numSegments = _way->getNumSegments();
    if(numSegments > 0)
        return _way->getSegment(RNG::intUniform(0, numSegments-1));
    return nullptr;
}

Lane* Map::getRandomLane(Segment *_segment)
{
    int numLanes = _segment->getNumLanes();
    if(numLanes > 0)
        return _segment->getLane(RNG::intUniform(0, numLanes-1));
    return nullptr;
}

std::vector<Node*> Map::getIntersectionNodes()
{
    std::vector<Node*> nodes;
    std::map<std::string,Node*>::iterator it;
    for(it=m_nodes.begin(); it!=m_nodes.end(); it++)
    {
        Node *node = it->second;
        if(node->isIntersectionNode())
        {
            nodes.push_back(node);
        }
    }

    return nodes;
}

std::vector<Node*> Map::getNodesList()
{
    std::vector<Node*> nodes;
    std::map<std::string,Node*>::iterator it;
    for(it=m_nodes.begin(); it!=m_nodes.end(); it++)
    {
        Node *node = it->second;
        nodes.push_back(node);
    }

    return nodes;
}

RoutingGraph Map::createGraph(Node *_from, Node *_to, Node *_previous)
{
    RoutingGraph graph;
    std::vector<Node*> nodes = getNodesList();
    for(unsigned int i=0; i<nodes.size(); i++)
    {
        Node *node = nodes.at(i);

        std::vector<Segment*> segments = node->getOutgoingSegments();
        for(unsigned int s=0; s<segments.size(); s++)
        {
            Segment *segment = segments.at(s);
            Node *neighbor = segment->getOtherNode(node);

            if(!(node==_from && neighbor==_previous))
               graph.addConnection(node, neighbor, segment->getLength());
        }
    }

    return graph;
}

std::vector<Node*> Map::convertRoutingPath(const RoutingPath &_path, Node *_previous)
{
    std::vector<Node*> path;
    if(_path.size()>1)
    {
        Node *node = _path.at(0)->getNode();
        path.push_back(node);
        for(unsigned int i=1; i<_path.size(); i++)
        {
            Node *currentNode = _path.at(i)->getNode();

            path.push_back(currentNode);
            node = currentNode;
        }
    }


    return path;
}

/*************************************
 *         CONVENIENCE METHODS       *
 ************************************/

Node* Map::getNode(const std::string &_id)
{
    if(m_nodes.count(_id)>0)
        return m_nodes[_id];
    return nullptr;
}

Way* Map::getWay(const std::string &_id)
{
    if(m_ways.count(_id)>0)
        return m_ways[_id];
    return nullptr;
}

Car* Map::getCar(const std::string &_id)
{
    return m_cars[_id];
}

std::string Map::int2string(int _value)
{
    std::stringstream stream;
    stream << _value;

    return stream.str();
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

std::map<std::string,Node*>& Map::getNodes()
{
    return m_nodes;
}

std::map<std::string,Way*>& Map::getWays()
{
    return m_ways;
}

std::map<std::string,Car*>& Map::getCars()
{
    return m_cars;
}

}
