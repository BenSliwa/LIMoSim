#ifndef LIMOSIM_OSMWAYENTRY_H
#define LIMOSIM_OSMWAYENTRY_H

#include "osmentry.h"
#include "osmnodeentry.h"
#include "LIMoSim/map/way.h"


namespace LIMoSim
{

class OSMWayEntry : public OSMEntry
{
public:
    OSMWayEntry(OSMDocument *_parent = 0);

    static OSMWayEntry fromXML(DOMElement *_dom, OSMDocument *_parent = 0);
    static OSMWayEntry fromWay(Way *_way, OSMDocument *_parent = 0);

    void parseTag(const std::string &_key, const Variant &_value);

    Way* toWay(std::map<std::string,OSMNodeEntry> &_nodes);
    DOMElement* toXML() const;

public:
    std::string id;
    std::vector<std::string> nodes;
    std::string highway;
    std::string name;
    double maxspeed;
    bool oneway = false;
    int lanes = 2;
    int forwardLanes = 1;
    int backwardLanes = 1;
};

}

#endif // LIMOSIM_OSMWAYENTRY_H
