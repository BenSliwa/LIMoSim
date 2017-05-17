#include "carentry.h"
#include "core/mobility/mobilitymodels.h"

namespace SCSIM
{

CarEntry::CarEntry(OSMDocument *_parent) :
    OSMEntry(_parent)
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

CarEntry CarEntry::fromXML(DOMElement *_entry, OSMDocument *_parent)
{
    CarEntry entry(_parent);

    entry.id = _entry->getAttribute("id").toString();
    entry.way = _entry->getAttribute("way").toString();
    entry.segment = _entry->getAttribute("segment").toInt();
    entry.lane = _entry->getAttribute("lane").toInt();
    entry.offset_m = _entry->getAttribute("offset").toInt();

    for(unsigned int i=0; i<_entry->childNodes.size(); i++)
    {
        DOMElement *child = _entry->childNodes.at(i)->toElement();
        std::string name = child->tagName;

        if(name=="tag")
        {
            std::string key = child->getAttribute("k").toString();
            Variant value = child->getAttribute("v");

            if(key=="speed")
                entry.speed_mps = value.toDouble() / 3.6;
            else if(key=="maxSpeed")
                entry.maxSpeed_mps = value.toDouble() / 3.6;
        }
        else if(name=="path")
        {
            for(unsigned int j=0; j<child->childNodes.size(); j++)
            {
                DOMElement *element = child->childNodes.at(j)->toElement();
                std::string node = element->getAttribute("ref").toString();

                if(j==0)
                    entry.currentNode = node;
                else
                    entry.path.push_back(node);
            }
        }
        else if(name=="mobilityModel")
        {
            for(unsigned int j=0; j<child->childNodes.size(); j++)
            {
                entry.mobilityModel = child->getAttribute("type").toString();

                DOMElement *element = child->childNodes.at(j)->toElement();
                std::string node = element->getAttribute("ref").toString();

                entry.trip.push_back(node);
            }
        }
    }

    return entry;
}

CarEntry CarEntry::fromCar(Car *_car, OSMDocument *_parent)
{
    CarEntry entry(_parent);

    PositionInfo info = _car->getPositionInfo();
    Lane *lane = info.lane;
    Segment *segment = lane->getSegment();
    Way *way = segment->getWay();

    entry.id = _car->getId();
    entry.way = way->getId();
    entry.segment = way->getSegmentIndex(segment);
    entry.lane = segment->getLaneIndex(lane);
    entry.offset_m = info.laneOffset_m;

    entry.speed_mps = _car->getSpeed();
    entry.maxSpeed_mps = _car->getMaxSpeed();

    entry.currentNode = info.currentNode->getId();
    for(unsigned int i=0; i<info.path.size(); i++)
    {
        Node *node = info.path.at(i);
        entry.path.push_back(node->getId());
    }

    // TODO: mobility model



    return entry;
}

Car* CarEntry::toCar() const
{
    Map *map = Map::getInstance();

    Car *car = map->createCar(id);

    Way *currentWay = map->getWay(way);
    Segment *currentSegment = currentWay->getSegments().at(segment);
    Lane *currentLane = currentSegment->getLane(lane);

    car->setSpeed(speed_mps);
    car->setMaxSpeed(maxSpeed_mps);

    PositionInfo info;
    info.lane = currentLane;
    info.alignment_m = 0;
    info.laneOffset_m = offset_m;
    info.currentNode = map->getNode(currentNode);

    for(unsigned int i=0; i<path.size(); i++)
    {
        Node *node = map->getNode(path.at(i));
        info.path.push_back(node);
    }

    car->setPositionInfo(info);
    car->setPosition(car->computeLanePosition(info.laneOffset_m, info.alignment_m, true));

    //
    if(mobilityModel=="FollowLane")
    {
        FollowLane *followLaneModel = new FollowLane(car);
        car->setMobilityModel(followLaneModel);
    }
    else if(mobilityModel=="Trip")
    {
        Trip *tripModel = new Trip(car);
        for(unsigned int i=0; i<trip.size(); i++)
        {
            Node *node = map->getNode(trip.at(i));
            tripModel->addTripEntry(node);
        }

        car->setMobilityModel(tripModel);
    }

    //
    car->start();


    return car;
}

DOMElement* CarEntry::toXML() const
{
    DOMElement *xml = new DOMElement("Car");

    xml->setAttribute("id", id);
    xml->setAttribute("way", way);
    xml->setAttribute("segment", segment);
    xml->setAttribute("lane", lane);
    xml->setAttribute("offset", offset_m);



    addTag("speed", Variant(speed_mps * 3.6), xml);
    addTag("maxSpeed", Variant(maxSpeed_mps * 3.6), xml);


    DOMElement *pathElement = new DOMElement("Path");
    xml->appendChild(pathElement);

    DOMElement *node = new DOMElement("nd");
    node->setAttribute("ref", currentNode);
    pathElement->appendChild(node);

    for(unsigned int i=0; i<path.size(); i++)
    {
        DOMElement *node = new DOMElement("nd");
        node->setAttribute("ref", Variant(path.at(i)));
        pathElement->appendChild(node);
    }

    // TODO: mobility model

    return xml;
}

}
