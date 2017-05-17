#ifndef LIMOSIM_OSMWAYENTRY_H
#define LIMOSIM_OSMWAYENTRY_H

#include "osmentry.h"
#include "osmnodeentry.h"
#include "core/map/way.h"


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
    bool oneway;
    int lanes;
    int forwardLanes;
    int backwardLanes;
};

}

#endif // LIMOSIM_OSMWAYENTRY_H
