#ifndef LIMOSIM_OSMNODEENTRY_H
#define LIMOSIM_OSMNODEENTRY_H

#include "osmentry.h"
#include "core/map/node.h"


namespace LIMoSim
{

class OSMNodeEntry : public OSMEntry
{
public:
    OSMNodeEntry(OSMDocument *_parent = 0);

    static OSMNodeEntry fromXML(DOMElement *_entry, OSMDocument *_parent = 0);
    static OSMNodeEntry fromNode(Node *_node, OSMDocument *_parent = 0);

    Node* toNode() const;
    DOMElement* toXML() const;

    void info() const;


public:
    std::string id;
    double lat;
    double lon;
    double x;
    double y;
    Position position;

    int type;
    std::string trafficSignalDirection;
};

}



#endif // LIMOSIM_OSMNODEENTRY_H
