#ifndef LIMOSIM_OSMBOUNDSENTRY_H
#define LIMOSIM_OSMBOUNDSENTRY_H

#include "osmentry.h"
#include "osmnodeentry.h"
#include "LIMoSim/map/way.h"


namespace LIMoSim
{

class OSMBoundsEntry : public OSMEntry
{
public:
    OSMBoundsEntry(OSMDocument *_parent = 0);

    static OSMBoundsEntry fromXML(DOMElement *_dom, OSMDocument *_parent = 0);
    static OSMBoundsEntry fromWay(Way *_way, OSMDocument *_parent = 0);

    void parseTag(const std::string &_key, const Variant &_value);

    DOMElement* toXML() const;

public:
    double minlat;
    double minlon;
    double maxlat;
    double maxlon;
};

}

#endif // LIMOSIM_OSMBOUNDSENTRY_H
