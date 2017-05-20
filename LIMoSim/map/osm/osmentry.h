#ifndef LIMOSIM_OSMENTRY_H
#define LIMOSIM_OSMENTRY_H

#include <iostream>
#include <map>
#include "LIMoSim/map/map.h"
#include "LIMoSim/settings/domelement.h"


namespace LIMoSim
{
class OSMDocument;

class OSMEntry
{
public:
    OSMEntry(OSMDocument *_parent = 0);

    static void addTag(const std::string &_key, const Variant &_value, DOMElement *_xml);

protected:
    OSMDocument *p_parent;


};

}

#endif // LIMOSIM_OSMENTRY_H
