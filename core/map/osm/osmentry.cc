#include "osmentry.h"

namespace LIMoSim
{

OSMEntry::OSMEntry(OSMDocument *_parent) :
    p_parent(_parent)
{

}

void OSMEntry::addTag(const std::string &_key, const Variant &_value, DOMElement *_xml)
{
    DOMElement *tag = new DOMElement("tag");
    tag->setAttribute("k", _key);
    tag->setAttribute("v", _value);
    _xml->appendChild(tag);
}

}
