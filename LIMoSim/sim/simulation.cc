#include "simulation.h"
#include "LIMoSim/map/nodegrid.h"

#include "LIMoSim/map/osm/vehicleparser.h"
#include "LIMoSim/settings/xmlparser.h"
#include "LIMoSim/vehicles/car.h"
#include "LIMoSim/mobility/mobilitymodels.h"
#include "LIMoSim/mobility/followerModel/followermodels.h"
#include "LIMoSim/mobility/laneChange/lanechangemodels.h"

#include "LIMoSim/map/osm/osmdocument.h"
#include "LIMoSim/map/osm/vehicleentry.h"



namespace LIMoSim
{

Simulation *simulationInstance = 0;

Simulation::Simulation(EventScheduler *_scheduler)
{
    if(_scheduler)
    {
        m_eventScheduler = _scheduler;

    }
    simulationInstance = this;


}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

Simulation* Simulation::getInstance(EventScheduler *_scheduler)
{
    if(!simulationInstance)
        simulationInstance = new Simulation(_scheduler);

    return simulationInstance;
}

bool Simulation::hasInstance()
{
    return (simulationInstance!=0);
}

void Simulation::registerEventHandler(EventHandler *_eventHandler)
{
    m_eventHandler.push_back(_eventHandler);
}

void Simulation::deregisterEventHandler(EventHandler *_eventHandler)
{
    for(unsigned int i=0; i<m_eventHandler.size(); i++)
    {
        EventHandler *handler = m_eventHandler.at(i);
        if(handler==_eventHandler)
        {
            m_eventHandler.erase(m_eventHandler.begin() + i);
            break;
        }
    }
}

void Simulation::load(const std::string &_map, const std::string &_vehicles)
{
    std::cout << "Simulation::load " << _map << "\t" << _vehicles << std::endl;

    Map *map = Map::getInstance();

    //
    std::string optPath = _map + ".limo";
    XMLParser xml;
    if(FileHandler::exists(optPath))
    {
        OSMDocument document = OSMDocument::fromXML(xml.parse(optPath));
    }
    else if(FileHandler::exists(_map))
    {
        OSMDocument document = OSMDocument::fromXML(xml.parse(_map));
        FileHandler::write(document.toXML()->toString(), optPath);
    }



    //
    std::map<std::string,Node*>& nodes = map->getNodes();
    std::map<std::string,Node*>::iterator it;
    for(it=nodes.begin(); it!=nodes.end(); it++)
    {
        Node *node = it->second;
        node->computeDestinations();


        // TODO: why is this needed to display the nodes with offset after the initial setup?
        std::vector<Segment*> segments = node->getSegments();
        for(unsigned int i=0; i<segments.size(); i++)
            segments.at(i)->linkLanes();
    }

    map->linkSegments();




    if(FileHandler::exists(_vehicles))
    {
        DOMElement *element = xml.parse(_vehicles);
        for(unsigned int i=0; i<element->childNodes.size(); i++)
        {
            DOMElement *child = element->childNodes.at(i)->toElement();

            int num = 1;
            if(child->hasAttribute("num"))
                num = child->getAttribute("num").toInt();

            for(unsigned int j=0; j<num; j++)
            {
                VehicleEntry *entry = static_cast<VehicleEntry*>(child);

                Car *car = entry->toCar();

                if(j==num-1)
                    delete entry;
            }
        }
    }

    // TODO: do this at the right point
    for(unsigned int i=0; i<m_eventHandler.size(); i++)
    {
        EventHandler *handler = m_eventHandler.at(i);
        handler->initialize();
    }
}


void Simulation::createCars(int _number)
{
    Map *map = Map::getInstance();
    for(unsigned int i=0; i<_number; i++)
    {
        Car *car = map->createCar();

        car->setMobilityModel(new RandomDirection(car));        
        car->setFollowerModel(new IDM(car));
        car->setLaneChangeModel(new MOBIL(car));

        double factor = 1 + RNG::intUniform(-200, 200) / 1000.0;
        car->setSpeedBehaviorFactor(factor);

        map->setRandomPosition(car);
    }
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Simulation::setEventScheduler(EventScheduler *_scheduler)
{
    m_eventScheduler = _scheduler;
}

EventScheduler* Simulation::getEventScheduler()
{
    return m_eventScheduler;
}


/*************************************
 *         CONVENIENCE METHODS       *
 ************************************/



/*************************************
 *           PRIVATE METHODS         *
 ************************************/

}
