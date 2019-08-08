#include "osmwayentry.h"
#include "osmdocument.h"

namespace LIMoSim
{

OSMWayEntry::OSMWayEntry(OSMDocument *_parent) :
    OSMEntry(_parent)
{

}

OSMWayEntry OSMWayEntry::fromXML(DOMElement *_dom, OSMDocument *_parent)
{
    OSMWayEntry entry(_parent);

    entry.id = _dom->getAttribute("id").toString();

    for(auto & childNode : _dom->childNodes)
    {
        DOMElement *child = childNode->toElement();
        std::string name = child->tagName;
        if(name=="nd")
        {
            std::string nodeId = child->getAttribute("ref").toString();
            entry.nodes.push_back(nodeId);
        }
        else if(name=="tag")
        {
            std::string key = child->getAttribute("k").toString();
            Variant value = child->getAttribute("v");

            entry.parseTag(key, value);
        }
    }


    return entry;
}

OSMWayEntry OSMWayEntry::fromWay(Way *_way, OSMDocument *_parent)
{
    OSMWayEntry entry(_parent);

    entry.id = _way->getId();
    entry.name = _way->getName();
    entry.maxspeed = _way->getMaxSpeed()*3.6;
    entry.lanes = _way->getLanes();
    entry.forwardLanes = _way->getForwardLanes();
    entry.backwardLanes = _way->getBackwardLanes();

    if(_way->getType()==WAY_TYPE::PRIMARY_HIGHWAY)
        entry.highway = "primary";
    else if(_way->getType()==WAY_TYPE::RESIDENTIAL)
        entry.highway = "residential";

    std::vector<Node*> &nodes = _way->getNodes();
    for(auto & node : nodes)
        entry.nodes.push_back(node->getId());

    return entry;
}

void OSMWayEntry::parseTag(const std::string &_key, const Variant &_value)
{
    if(_key=="highway")
        highway = _value.toString();
    else if(_key=="name")
        name = _value.toString();
    else if(_key=="maxspeed")
        maxspeed = _value.toDouble();
    else if(_key=="oneway")
    {
        if(_value.toString()=="yes")
            oneway = true;
        else
            oneway = false;
    }

    /*lanes = 2;
    forwardLanes = 1;
    backwardLanes = 1;*/

    else if(_key=="lanes")
        lanes = _value.toInt();
    else if(_key=="lanes:forward")
        forwardLanes = _value.toInt();
    else if(_key=="lanes:backward")
        backwardLanes = _value.toInt();
}

Way* OSMWayEntry::toWay(std::map<std::string,OSMNodeEntry> &_nodes)
{
    //std::cout << "OSMWayEntry::toWay " << id << "\t" << nodes.size() << std::endl;

    Map *map = Map::getInstance();
    Way *way = map->createWay(id);
    if(highway=="residential")
        way->setType(WAY_TYPE::RESIDENTIAL);
    else
        way->setType(WAY_TYPE::PRIMARY_HIGHWAY);

    way->setName(name);
    way->setMaxSpeed(maxspeed/3.6);
    way->setIsOneway(oneway);
    way->setLanes(lanes);
    way->setForwardLanes(forwardLanes);
    way->setBackwardLanes(backwardLanes);

    for(auto id : nodes)
    {
        OSMNodeEntry entry = _nodes[id];

        Node *node = map->getNode(id);
        if(!node)
            node = entry.toNode();

        way->addNode(node);
    }

    way->linkLanes();

    return way;
}

DOMElement* OSMWayEntry::toXML() const
{
    DOMElement *xml = new DOMElement("way");

    xml->setAttribute("id", id);

    for(const auto & i : nodes)
    {
        DOMElement *node = new DOMElement("nd");
        node->setAttribute("ref", Variant(i));
        xml->appendChild(node);
    }

    addTag("highway", Variant(highway), xml);
    addTag("name", Variant(name), xml);
    addTag("maxspeed", maxspeed, xml);
    addTag("lanes", lanes, xml);
    addTag("lanes:forward", forwardLanes, xml);
    addTag("lanes:backward", backwardLanes, xml);

    return xml;
}

}
