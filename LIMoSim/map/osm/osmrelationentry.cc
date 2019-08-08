#include "osmrelationentry.h"

namespace LIMoSim
{

OSMRelationEntry::OSMRelationEntry(OSMDocument *_parent) :
    OSMEntry(_parent)
{

}

OSMRelationEntry OSMRelationEntry::fromXML(DOMElement *_entry, OSMDocument *_parent)
{
    OSMRelationEntry entry(_parent);

    entry.id = _entry->getAttribute("id").toInt();
    for(auto & childNode : _entry->childNodes)
    {
        DOMElement *child = childNode->toElement();
        std::string name = child->tagName;

        if(name=="tag")
        {
            std::string key = child->getAttribute("k").toString();
            Variant value = child->getAttribute("v");

            if(key=="name")
                entry.name = value.toString();
            else if(key=="type")
                entry.type = value.toString();
        }
        else if(name=="member")
        {
            std::string role = child->getAttribute("role").toString();
            std::string ref = child->getAttribute("ref").toString();

            if(role=="house")
                entry.houses.push_back(ref);
            else if(role=="street")
                entry.streets.push_back(ref);
        }
    }

    return entry;
}

}
