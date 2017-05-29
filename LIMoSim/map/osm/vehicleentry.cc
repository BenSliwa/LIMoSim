#include "vehicleentry.h"
#include "LIMoSim/mobility/mobilitymodels.h"
#include "LIMoSim/mobility/followerModel/followermodels.h"
#include "LIMoSim/mobility/laneChange/lanechangemodels.h"

namespace LIMoSim
{

VehicleEntry::VehicleEntry() :
    DOMElement("Car")
{

}

VehicleEntry* VehicleEntry::fromCar(Car *_car)
{
    VehicleEntry *entry = new VehicleEntry();

    PositionInfo info = _car->getPositionInfo();
    Lane *lane = info.lane;
    Segment *segment = lane->getSegment();
    Way *way = segment->getWay();

    entry->setAttribute("id", _car->getId());
    entry->setAttribute("lane", segment->getLaneIndex(lane));
    entry->setAttribute("segment", way->getSegmentIndex(segment));
    entry->setAttribute("way", way->getId());
    entry->setAttribute("offset", info.laneOffset_m);

    entry->addTag("speed", _car->getSpeed() * 3.6);
    entry->addTag("maxSpeed", _car->getMaxSpeed() * 3.6);



    //
    DOMElement *path = new DOMElement("path");
    info.path.insert(info.path.begin(), info.currentNode);
    for(unsigned int i=0; i<info.path.size(); i++)
    {
        std::string id = info.path.at(i)->getId();
        DOMElement *node = new DOMElement("nd");
        node->setAttribute("ref", id);
        path->appendChild(node);
    }
    entry->appendChild(path);




    //
    DOMElement *mobilityModelEntry = new DOMElement("mobilityModel");
    MobilityModel *mobilityModel = _car->getMobilityModel();
    mobilityModelEntry->setAttribute("type", mobilityModel->getType());
    entry->appendChild(mobilityModelEntry);

    //
    DOMElement *followerModelEntry = new DOMElement("followerModel");
    FollowerModel *followerModel = _car->getFollowerModel();
    followerModelEntry->setAttribute("type", followerModel->getType());
    entry->appendChild(followerModelEntry);


    //
    DOMElement *laneChangeModelEntry = new DOMElement("laneChangeModel");
    LaneChangeModel *laneChangeModel = _car->getLaneChangeModel();
    laneChangeModelEntry->setAttribute("type", laneChangeModel->getType());
    entry->appendChild(laneChangeModelEntry);

    return entry;
}

void VehicleEntry::addTag(const std::string &_key, const Variant &_value)
{
    DOMElement *tag = new DOMElement("tag");
    tag->setAttribute("k", _key);
    tag->setAttribute("v", _value);
    appendChild(tag);
}

Car* VehicleEntry::toCar()
{
    Map *map = Map::getInstance();

    Car *car = map->createCar(getAttribute("id").toString());


    PositionInfo info;
    if(hasAttribute("way"))
    {
        Way *currentWay = map->getWay(getAttribute("way").toString());
        Segment *currentSegment = currentWay->getSegments().at(getAttribute("segment").toInt());
        info.lane = currentSegment->getLane(getAttribute("lane").toInt());

        info.alignment_m = 0;
        info.laneOffset_m = getAttribute("offset").toDouble();
    }
    else
    {
        map->setRandomPosition(car);
    }



    for(unsigned int i=0; i<childNodes.size(); i++)
    {
        DOMElement *child = childNodes.at(i)->toElement();
        std::string tag = child->tagName;
        if(tag=="tag")
        {
            std::string key = child->getAttribute("k").toString();
            Variant value = child->getAttribute("v");

            if(key=="speed")
                car->setSpeed(value.toDouble() / 3.6);
            else if(key=="maxSpeed")
                car->setMaxSpeed(value.toDouble() / 3.6);

        }
        else if(tag=="path")
        {
            for(unsigned int j=0; j<child->childNodes.size(); j++)
            {
                DOMElement *ref = child->childNodes.at(j)->toElement();
                std::string id = ref->getAttribute("ref").toString();
                Node *node = map->getNode(id);

                if(j==0)
                    info.currentNode = node;
                else
                    info.path.push_back(node);

            }
        }
        else if(tag=="mobilityModel")
        {
            std::string type = child->getAttribute("type").toString();
            if(type=="Trip")
            {
                Trip *trip = new Trip(car);

                for(unsigned int j=0; j<child->childNodes.size(); j++)
                {
                    DOMElement *ref = child->childNodes.at(j)->toElement();
                    std::string id = ref->getAttribute("ref").toString();
                    Node *node = map->getNode(id);

                    trip->addTripEntry(node);
                }

                car->setMobilityModel(trip);
            }
            else if(type=="RandomDirection")
            {
                RandomDirection *followLane = new RandomDirection(car);
                car->setMobilityModel(followLane);
            }
        }
        else if(tag=="followerModel")
        {
            std::string type = child->getAttribute("type").toString();
            if(type=="IDM")
            {
                IDM *idm = new IDM(car);
                car->setFollowerModel(idm);
            }
        }
        else if(tag=="laneChangeModel")
        {
            std::string type = child->getAttribute("type").toString();
            if(type=="MOBIL")
            {
                MOBIL *mobil = new MOBIL(car);
                car->setLaneChangeModel(mobil);
            }
        }
    }


    if(hasAttribute("way"))
    {
        car->setPositionInfo(info);
        car->setPosition(car->computeLanePosition(info.laneOffset_m, info.alignment_m, true));
    }

    return car;
}

}
