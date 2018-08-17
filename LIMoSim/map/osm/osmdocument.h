#ifndef LIMOSIM_OSMDOCUMENT_H
#define LIMOSIM_OSMDOCUMENT_H

#include "osmboundsentry.h"
#include "osmnodeentry.h"
#include "osmwayentry.h"
#include "osmrelationentry.h"

#include "LIMoSim/map/map.h"
#include "LIMoSim/map/node.h"
#include "LIMoSim/map/way.h"
#include "LIMoSim/map/trafficsignal.h"

#include "LIMoSim/location/wgs84.h"

namespace LIMoSim
{

struct Bounds
{
    bool valid = false;
    double minLat = std::numeric_limits<double>::max();
    double maxLat = std::numeric_limits<double>::min();
    double minLon  = std::numeric_limits<double>::max();
    double maxLon = std::numeric_limits<double>::min();

    Position getOrigin(){return Position(minLon, minLat);}
};

class OSMDocument
{
public:
    OSMDocument();

    static OSMDocument fromXML(DOMElement *_entry, IGeoCoordConverter &geoCoordConverter);
    void createOSMEntries(DOMElement *_entry);
    void adjustNodePositions(IGeoCoordConverter &geoCoordConverter);
    void adjustBounds(const OSMNodeEntry &_node);
    void createRelations();
    void createWays();

    bool hasWay(const std::string &_id);
    bool hasNode(const std::string &_id);

    DOMElement* toXML();

public:
    bool useWgs;

private:
    Map *p_map;
    Bounds m_bounds;

    OSMBoundsEntry m_boundsentry;
    std::map<std::string, OSMNodeEntry> m_nodes;
    std::map<std::string, OSMWayEntry> m_ways;
    std::vector<OSMRelationEntry> m_relations;

};

}

#endif // LIMOSIM_OSMDOCUMENT_H
