#include "osmdocument.h"

namespace LIMoSim
{

OSMDocument::OSMDocument() :
    useWgs(true),
    p_map(Map::getInstance())
{

}

OSMDocument OSMDocument::fromXML(DOMElement *_entry)
{
    OSMDocument document;

    std::string generator = _entry->getAttribute("generator").toString();
    if(generator=="LIMoSim")
        document.useWgs = false;

    document.createOSMEntries(_entry);

    WGS84 wgs84;
    document.adjustNodePositions(wgs84);

    if(document.useWgs)
    {
        document.createRelations();
        //document.createWays();
    }
    else
        document.createWays();


    return document;
}

void OSMDocument::createOSMEntries(DOMElement *_entry)
{
    for(unsigned int i=0; i<_entry->childNodes.size(); i++)
    {
        DOMElement *element = _entry->childNodes.at(i)->toElement();
        std::string name = element->tagName;

        if(name=="node")
        {
            OSMNodeEntry entry = OSMNodeEntry::fromXML(element, this);
            std::stringstream id; id<<entry.id;

            m_nodes[id.str()] = entry;
        }
        else if(name=="relation")
        {
            OSMRelationEntry entry = OSMRelationEntry::fromXML(element, this);
            if(entry.type=="associatedStreet")
            {

            }
            m_relations.push_back(entry);
        }
        else if(name=="way")
        {
            OSMWayEntry entry = OSMWayEntry::fromXML(element, this);
            m_ways[entry.id] = entry;
        }
        else if(name=="controller")
        {
           // parseController(element);
        }
        else if(name=="bounds")
        {
            m_boundsentry = OSMBoundsEntry::fromXML(element, this);
            m_bounds.minLat = m_boundsentry.minlat;
            m_bounds.minLon = m_boundsentry.minlon;
            m_bounds.maxLat = m_boundsentry.maxlat;
            m_bounds.maxLon = m_boundsentry.maxlon;
        }
    }
}

void OSMDocument::adjustNodePositions(IGeoCoordConverter &geoCoordConverter)
{
    std::cout << "OSMDocument::adjustNodePositions " << useWgs << "\tn: " << m_nodes.size() << std::endl;

    if(useWgs) // TODO: rel getNodeEntries
    {
        for(unsigned int i=0; i<m_relations.size(); i++)
        {
            OSMRelationEntry &relation = m_relations.at(i);
            for(unsigned int w=0; w<relation.streets.size(); w++)
            {
                std::string wayId = relation.streets.at(w);

                if(hasWay(wayId))
                {
                    OSMWayEntry way = m_ways[wayId];
                    for(unsigned int n=0; n<way.nodes.size(); n++)
                    {
                        std::string nodeId = way.nodes.at(n);
                        if(hasNode(nodeId))
                        {
                            OSMNodeEntry node = m_nodes[nodeId];
                            adjustBounds(node);
                        }


                    }
                }

            }
        }
    }



/*
    std::map<std::string, OSMNodeEntry>::iterator n;
    for(n=m_nodes.begin(); n!=m_nodes.end(); n++)
    {
        OSMNodeEntry entry = n->second;
        adjustBounds(entry);
    }*/





    Position bottomLeft(m_bounds.minLon, m_bounds.minLat);
    Position topRight(m_bounds.maxLon, m_bounds.maxLat);
    geoCoordConverter.setOrigin(m_bounds.getOrigin());
    Vector3d offset = geoCoordConverter.getOffset(topRight);
    Vector3d s = (topRight-bottomLeft);

    std::cout << "wgs: " << offset.toString() << "\tcartesian: " << s.toString() << std::endl;
    std::cout << "DX: " << bottomLeft.toString() << "\t" << topRight.toString() << std::endl;


    // nodes preprocessing
    std::map<std::string, OSMNodeEntry>::iterator it;
    for(it=m_nodes.begin(); it!=m_nodes.end(); it++)
    {
        OSMNodeEntry &_entry = it->second;
        if(useWgs)
        {
            Vector3d offset = geoCoordConverter.getOffset(Position(_entry.lon, _entry.lat));
            _entry.position = offset;
        }
        else
            _entry.position = Position(_entry.x, _entry.y);

    }

    std::cout << "n: " << p_map->getNodes().size() << "\tw: " << p_map->getWays().size() << std::endl;
    std::cout << "osm: n: " << m_nodes.size() << "\tw: " << m_ways.size() << std::endl;
}

void OSMDocument::adjustBounds(const OSMNodeEntry &_node)
{
}

void OSMDocument::createRelations()
{
    std::cout << "OSMDocument::createRelations" << std::endl;

    for(unsigned int i=0; i<m_relations.size(); i++)
    {
        OSMRelationEntry entry = m_relations.at(i);
       // std::cout << "relation: " << entry.name << std::endl;

        for(unsigned int w=0; w<entry.streets.size(); w++)
        {
            std::string wayId = entry.streets.at(w);

            if(hasWay(wayId))
            {
                OSMWayEntry wayEntry = m_ways[wayId];

                std::string type = wayEntry.highway;
                if(!(type=="path" || type=="service" || type=="steps" || type=="pedestrian" || type=="footway"))
                {
                    Way *way = p_map->getWay(wayEntry.id);
                    if(!way)
                        way = wayEntry.toWay(m_nodes);
                }
            }


        }
    }
}

void OSMDocument::createWays()
{
    std::map<std::string, OSMWayEntry>::iterator it;
    for(it=m_ways.begin(); it!=m_ways.end(); it++)
    {
        OSMWayEntry entry = it->second;

        std::string type = entry.highway;
        if(!(type=="path" || type=="service" || type=="steps" || type=="pedestrian" || type=="footway"))
        {
            if(entry.highway=="primary" || entry.highway=="residential")
            {
                Way *way = p_map->getWay(entry.id);
                if(!way)
                    way = entry.toWay(m_nodes);
            }

        }
    }
}

bool OSMDocument::hasWay(const std::string &_id)
{
    return (m_ways.count(_id)>0);
}

bool OSMDocument::hasNode(const std::string &_id)
{
    return (m_nodes.count(_id)>0);
}

DOMElement* OSMDocument::toXML()
{
    DOMElement *xml = new DOMElement("osm");
    xml->setAttribute("generator", Variant("LIMoSim"));

    // bounds
    xml->appendChild(m_boundsentry.toXML());
    // nodes
    std::map<std::string,Node*> nodes = p_map->getNodes();
    std::map<std::string,Node*>::iterator n;
    for(n=nodes.begin(); n!=nodes.end(); n++)
    {
        Node *node = n->second;
        OSMNodeEntry entry = OSMNodeEntry::fromNode(node, this);

        xml->appendChild(entry.toXML());
    }

    // ways
    std::map<std::string,Way*>& ways = p_map->getWays();
    std::map<std::string,Way*>::iterator w;
    for(w=ways.begin(); w!=ways.end(); w++)
    {
        Way *way = w->second;
        OSMWayEntry entry = OSMWayEntry::fromWay(way, this);

        xml->appendChild(entry.toXML());
    }



    return xml;
}

}

