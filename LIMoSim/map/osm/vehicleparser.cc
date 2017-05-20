#include "vehicleparser.h"
#include "LIMoSim/vehicles/car.h"

namespace LIMoSim
{

VehicleParser::VehicleParser() :
    Parser(),
    p_map(Map::getInstance())
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


void VehicleParser::loadScenario(DOMElement *_dom)
{
    for(unsigned int i=0; i<_dom->childNodes.size(); i++)
    {
        DOMElement *element = _dom->childNodes.at(i)->toElement();
        std::string name = element->tagName;

        if(name=="Car")
            parseCar(element);
    }
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void VehicleParser::parseCar(DOMElement *_dom)
{
    std::string id = _dom->getAttribute("id").toString();

    Car *car = p_map->createCar(id);
    PositionInfo info = car->getPositionInfo();

    std::string wayId = _dom->getAttribute("way").toString();
    int segmentId = _dom->getAttribute("segment").toInt();
    int laneId = _dom->getAttribute("lane").toInt();

    Way *way = p_map->getWay(wayId);
    Segment *segment = way->getSegment(segmentId);
    Lane *lane = segment->getLanes().at(laneId);
    info.lane = lane;
    if(_dom->hasAttribute("offset"))
        info.laneOffset_m = _dom->getAttribute("offset").toDouble();

    for(unsigned int i=0; i<_dom->childNodes.size(); i++)
    {
        DOMElement *child = _dom->childNodes.at(i)->toElement();
        std::string name = child->tagName;

        if(name=="tag")
        {
            std::string key = child->getAttribute("k").toString();
            Variant value = child->getAttribute("v");

            parseCarTag(key, value, car);
        }
        else if(name=="Path")
        {
            std::vector<Node*> path = parsePath(child);
            if(path.size()>0)
            {
                info.currentNode = path.at(0);
                path.erase(path.begin());
                info.path = path;
            }
            else
            {
                // TODO: handle this
            }
        }
    }





    car->setPositionInfo(info);
    car->start();
}

void VehicleParser::parseCarTag(const std::string &_key, const Variant &_value, Car *_car)
{
    if(_key=="speed")
        _car->setSpeed(_value.toDouble()/3.6);
    else if(_key=="maxSpeed")
        _car->setMaxSpeed(_value.toDouble()/3.6);
}

std::vector<Node*> VehicleParser::parsePath(DOMElement *_dom)
{
    std::vector<Node*> path;

    for(unsigned int i=0; i<_dom->childNodes.size(); i++)
    {
        DOMElement *child = _dom->childNodes.at(i)->toElement();
        std::string name = child->tagName;

        if(name=="nd")
        {
            Node *node = p_map->getNode(child->getAttribute("ref").toString());
            path.push_back(node);
        }
    }


    return path;
}

}
