#include "simulation.h"
#include "core/map/nodegrid.h"

#include "core/map/osm/vehicleparser.h"
#include "core/settings/xmlparser.h"
#include "core/vehicles/car.h"
#include "core/mobility/mobilitymodels.h"
#include "core/mobility/followerModel/followermodels.h"
#include "core/mobility/laneChange/lanechangemodels.h"

#include "core/map/osm/osmdocument.h"
#include "core/map/osm/vehicleentry.h"



namespace LIMOSIM
{

Simulation *simulationInstance = 0;

Simulation::Simulation(EventScheduler *_scheduler)
{
    if(_scheduler)
    {
        m_eventScheduler = _scheduler;

    }
    simulationInstance = this;


    std::cout << "Simulation::Simulation" << std::endl;
    Map *map = Map::getInstance();



    std::string dir = "D:/QtProjects/LIMoSim/resources/";
    //dir = "C:/Users/Benjamin/Desktop/LIMoSim/resources/";
    //dir = "/home/sliwa/inet/inet/src/inet/LIMoSim/resources/";


    int type = 0;
    int cars  = 0;

    if(type==10)
    {
        int s = map->getNodes().size();
        if(s==0)
        {
            NodeGrid *grid = new NodeGrid();
            int s = 2;
            int w = 600;
            grid->init(s, w, s, w);

        }

        cars = 100;
    }
    else if(type==0)
    {
        std::string path = dir + "Test.osm";

        path = dir + "Hedreisch.osm";
        path = dir + "map.osm";
        path = dir + "streets.osm";
        //path = dir + "TUDO.osm";
        //path = dir + "TUDO.osm.opt";
        path = dir + "TUDO.osm";

        //path = dir + "Test.osm";


        std::string optPath = path + ".opt";

        XMLParser xml;

        if(FileHandler::exists(optPath))
        {
            OSMDocument document = OSMDocument::fromXML(xml.parse(optPath));
        }
        else
        {
            OSMDocument document = OSMDocument::fromXML(xml.parse(path));
            FileHandler::write(document.toXML()->toString(), optPath);
        }



        /*
        std::string vehicles = dir + "Vehicles.xml";
        VehicleParser vehicleParser;
        vehicleParser.loadScenario(xml.parse(vehicles));*/


        /*
        createCars(2);
        Car *c0 = map->getCar("1");
        c0->setMaxSpeed(10/3.6);
        c0->setSpeed(0);*/


    }
    if(type==2) // time/space eval
    {
        Way *way = map->createWay();
        way->addNode(map->createNode(Position(500, 900)));
        way->addNode(map->createNode(Position(1300, 900)));
    }

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



    if(type==2)
    {
        int cars = 20;

        Way *way = map->getWay("0");
        for(unsigned int i=0; i<cars; i++)
        {
            double offset_m = 7 * i; // 5m car + 2m offset

            Car *car = map->createCar();
            car->setMobilityModel(new FollowLane(car));
            car->setFollowerModel(new IDM(car));
            car->setLaneChangeModel(new MOBIL(car));


            PositionInfo info;

            info.lane = way->getSegment(0)->getLanes().at(1);

            info.currentNode = map->getNode("1");
            info.laneOffset_m = offset_m;
            car->setPositionInfo(info);

            double factor = 1 + RNG::intUniform(-200, 200) / 1000.0;
            car->setSpeedBehaviorFactor(factor);
            car->start();


            //car->setMaxSpeed(0);
        }

        Car *car = map->createCar();
        car->setMobilityModel(new FollowLane(car));
        car->setFollowerModel(new IDM(car));
        car->setLaneChangeModel(new MOBIL(car));

        PositionInfo info;
        info.lane = way->getSegment(0)->getLanes().at(1);
        info.currentNode = map->getNode("1");
        info.laneOffset_m = 700 - 7;
        car->setMaxSpeed(0);
        car->setPositionInfo(info);
        car->start();
    }






    XMLParser xml;
    DOMElement *element = xml.parse(dir + "Cars.xml"); //CarsLaneChange
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



   // createCars(100);

    /*
    createCars(2);
    Car *c0 = map->getCar("1");
    c0->setMaxSpeed(10/3.6);
    c0->setSpeed(0);*/

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

void Simulation::createCars(int _number)
{
    Map *map = Map::getInstance();
    for(unsigned int i=0; i<_number; i++)
    {
        Car *car = map->createCar();

        car->setMobilityModel(new FollowLane(car));        
        car->setFollowerModel(new IDM(car));
        car->setLaneChangeModel(new MOBIL(car));

        double factor = 1 + RNG::intUniform(-200, 200) / 1000.0;
        car->setSpeedBehaviorFactor(factor);

        map->setRandomPosition(car);

        car->start();
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
