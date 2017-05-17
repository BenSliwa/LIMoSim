#ifndef LIMOSIM_MAP_H
#define LIMOSIM_MAP_H

#include <map>
#include <vector>
#include <limits>
#include "core/location/position.h"

/*
 *  TODO: id history instead of map size
 *
 *
 *
 *
 */

namespace LIMoSim
{
class Car;
class Lane;
class Node;
class RoutingGraph;
class RoutingPath;
class Segment;
class TrafficSignal;
class Way;



class Map
{
public:
    Map();
    ~Map();

    static Map* getInstance();

    void clear();

    Node* createNode(const Position &_position, const std::string &_id="");
    void removeNode(Node *_node);
    void updateAllNodesWithDestination(Node *_destination);
    void linkSegments();

    Way* createWay(const std::string &_id="");
    Car* createCar(const std::string &_id="");

    void setRandomPosition(Car *_car);
    Way* getRandomWay();
    Segment* getRandomSegment(Way *_way);
    Lane* getRandomLane(Segment *_segment);


    // routing
    std::vector<Node*> getIntersectionNodes();
    std::vector<Node*> getNodesList();
    RoutingGraph createGraph(Node *_from, Node *_to, Node *_previous=0);
    std::vector<Node*> convertRoutingPath(const RoutingPath &_path, Node *_previous=0);

    // convenience methods
    Node* getNode(const std::string &_id);
    Way* getWay(const std::string &_id);
    Car* getCar(const std::string &_id);
    std::string int2string(int _value);



    // property accessors
    std::map<std::string,Node*>& getNodes();
    std::map<std::string,Way*>& getWays();
    std::map<std::string,Car*>& getCars();

private:
    std::map<std::string,Node*> m_nodes;
    std::map<std::string,Way*> m_ways;
    std::map<std::string,Car*> m_cars;

    int m_nodeId;
    int m_wayId;
};

}

#endif // MAP_H
