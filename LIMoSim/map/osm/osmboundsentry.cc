#include "osmboundsentry.h"
#include "osmdocument.h"

namespace LIMoSim
{

OSMBoundsEntry::OSMBoundsEntry(OSMDocument *_parent) :
    OSMEntry(_parent)
{

}

OSMBoundsEntry OSMBoundsEntry::fromXML(DOMElement *_entry, OSMDocument *_parent)
{
    OSMBoundsEntry entry(_parent);

    entry.minlat = _entry->getAttribute("minlat").toDouble();
    entry.minlon = _entry->getAttribute("minlon").toDouble();
    entry.maxlat = _entry->getAttribute("maxlat").toDouble();
    entry.maxlon = _entry->getAttribute("maxlon").toDouble();

    return entry;
}

DOMElement* OSMBoundsEntry::toXML() const
{
    DOMElement *xml = new DOMElement("bounds");

    xml->setAttribute("minlat", minlat);
    xml->setAttribute("minlon", minlon);
    xml->setAttribute("maxlat", maxlat);
    xml->setAttribute("maxlon", maxlon);

    return xml;
}

}
