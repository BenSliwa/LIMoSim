#include "osmnodeentry.h"
#include "LIMoSim/map/mapdefinitions.h"
#include "LIMoSim/map/trafficsignal.h"

namespace LIMoSim
{

OSMNodeEntry::OSMNodeEntry(OSMDocument *_parent) :
    OSMEntry(_parent)
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

OSMNodeEntry OSMNodeEntry::fromXML(DOMElement *_entry, OSMDocument *_parent)
{
    OSMNodeEntry entry(_parent);
    entry.id = _entry->getAttribute("id").toString();
    entry.lat = _entry->getAttribute("lat").toDouble();
    entry.lon = _entry->getAttribute("lon").toDouble();
    entry.x = _entry->getAttribute("x").toDouble();
    entry.y = _entry->getAttribute("y").toDouble();

    entry.type = NODE_TYPE::NODE;
    for(unsigned int i=0; i<_entry->childNodes.size(); i++)
    {
        DOMElement *child = _entry->childNodes.at(i)->toElement();
        std::string name = child->tagName;

        if(name=="tag")
        {
            std::string key = child->getAttribute("k").toString();
            Variant value = child->getAttribute("v");

            if(value.toString()=="traffic_signals")
                entry.type = NODE_TYPE::TRAFFIC_SIGNAL;
            else if(key=="traffic_signals:direction")
                entry.trafficSignalDirection = value.toString();
        }
    }


    return entry;
}

OSMNodeEntry OSMNodeEntry::fromNode(Node *_node, OSMDocument *_parent)
{
    OSMNodeEntry entry(_parent);

    entry.id = _node->getId();
    entry.type = _node->getType();

    TrafficSignal *signal = _node->getTrafficSignal();
    if(signal)
    {
        int direction = signal->getDirection();

        if(direction==WAY_DIRECTION::FORWARD)
            entry.trafficSignalDirection = "forward";
        else if(direction==WAY_DIRECTION::BACKWARD)
            entry.trafficSignalDirection = "backward";
    }


    Position position = _node->getPosition();
    entry.x = position.x;
    entry.y = position.y;

    return entry;
}

Node* OSMNodeEntry::toNode() const
{
    Node *node = Map::getInstance()->createNode(position, id);
    node->setType(type);

    if(type==NODE_TYPE::TRAFFIC_SIGNAL)
    {
        TrafficSignal *signal = new TrafficSignal(node);
        node->setTrafficSignal(signal);

        if(trafficSignalDirection=="forward")
            signal->setDirection(WAY_DIRECTION::FORWARD);
        else if(trafficSignalDirection=="backward")
            signal->setDirection(WAY_DIRECTION::BACKWARD);
    }

    return node;
}

DOMElement* OSMNodeEntry::toXML() const
{
    DOMElement *xml = new DOMElement("node");

    xml->setAttribute("id", id);
    xml->setAttribute("x", x);
    xml->setAttribute("y", y);

    if(type==NODE_TYPE::TRAFFIC_SIGNAL)
    {
        addTag("highway", Variant("traffic_signals"), xml);

        if(trafficSignalDirection!="")
            addTag("traffic_signals:direction", Variant(trafficSignalDirection), xml);
    }

    return xml;
}

void OSMNodeEntry::info() const
{
    std::cout << "INFO: " << id << "\t" << lat << "\t" << lon << "\t" << x << "\t" << y << std::endl;
}

}
