//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "LIMoSimCar.h"

#include "core/map/map.h"
#include "core/map/nodegrid.h"
#include "followerModel/FollowerModel.h"
#include "strategicModel/StrategicModel.h"

#include "core/sim/simulation.h"
#include "omnet/sim/EventScheduler.h"

#include "core/settings/xmlparser.h"
#include "core/map/osm/vehicleentry.h"

namespace inet {

Define_Module(LIMoSimCar);

LIMoSimCar::LIMoSimCar()
{

}

LIMoSimCar::~LIMoSimCar()
{

    // TODO: delete the car with the map
}

void LIMoSimCar::initialize(int _stage)
{
    MovingMobilityBase::initialize(_stage);

}

void LIMoSimCar::setInitialPosition()
{
    if(!LIMoSim::Simulation::hasInstance())
    {
        EventScheduler* scheduler = EventScheduler::getInstance();
        LIMoSim::Simulation *sim = LIMoSim::Simulation::getInstance(scheduler);
    }

    LIMoSim::Map *map = LIMoSim::Map::getInstance();

    bool useXML = true;

    if(useXML)
    {
        std::string dir =  "/home/sliwa/inet/inet/src/inet/LIMoSim/resources/";
        std::string file = dir + "Trip.xml";

        LIMoSim::XMLParser xml;
        LIMoSim::VehicleEntry *entry = static_cast<LIMoSim::VehicleEntry*>(xml.parse(file));

        p_car = entry->toCar();
    }
    else
    {
        p_car = map->createCar();


        //
        StrategicModel *strategicModel_omnet = dynamic_cast<StrategicModel*>(getSubmodule("strategicModel"));
        LIMoSim::MobilityModel *strategicModel = strategicModel_omnet->createStrategicModel(p_car);
        p_car->setMobilityModel(strategicModel);


        //
        FollowerModel *model = dynamic_cast<FollowerModel*>(getSubmodule("followerModel"));
        LIMoSim::FollowerModel *followerModel = model->createFollowerModel(p_car);
        p_car->setFollowerModel(followerModel);
        p_car->start();



        map->setRandomPosition(p_car);
    }



    updatePosition();

}

void LIMoSimCar::move()
{
    // we are not moving here, just updating the ui
    if(p_car)
        updatePosition();
}

void LIMoSimCar::updatePosition()
{
    if(p_car)
    {
        LIMoSim::Position position = p_car->getPosition();
        Coord coord(position.x, position.y, position.z);

        lastPosition = coord;
    }
}

}
