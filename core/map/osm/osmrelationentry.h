#ifndef OSMRELATIONENTRY_H
#define OSMRELATIONENTRY_H

#include "osmentry.h"

namespace LIMOSIM
{

class OSMRelationEntry : public OSMEntry
{
public:
    OSMRelationEntry(OSMDocument *_parent = 0);

    static OSMRelationEntry fromXML(DOMElement *_entry, OSMDocument *_parent = 0);


public:
    long long id;
    std::string name;
    std::string type;

    std::vector<std::string> streets;
    std::vector<std::string> houses;

};

}

#endif // OSMRELATIONENTRY_H
