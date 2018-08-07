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

#include "LIMoSim/map/map.h"
#include "LIMoSim/map/nodegrid.h"
#include "followerModel/FollowerModel.h"
#include "strategicModel/StrategicModel.h"

#include "LIMoSim/sim/simulation.h"
#include "omnet/sim/LIMoSimController.h"

#include "LIMoSim/settings/xmlparser.h"
#include "LIMoSim/map/osm/vehicleentry.h"

#include "LIMoSim/mobility/laneChange/mobil.h"

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
    std::string mapFile = par("map").stringValue();

    if(!LIMoSim::Simulation::hasInstance())
    {
        LIMoSimController* scheduler = LIMoSimController::getInstance();
        LIMoSim::Simulation *sim = LIMoSim::Simulation::getInstance(scheduler);
        sim->load(mapFile, "");
    }

    LIMoSim::Map *map = LIMoSim::Map::getInstance();

    StrategicModel *strategicModel_omnet = dynamic_cast<StrategicModel*>(getSubmodule("strategicModel"));
    FollowerModel *followerModel_omnet = dynamic_cast<FollowerModel*>(getSubmodule("followerModel"));




    std::string configuration = par("configuration").stringValue();
    if(configuration!="")
    {
        std::cout << "LIMoSimCar::setInitialPosition from XML" << std::endl;

        LIMoSim::XMLParser xml;
        LIMoSim::VehicleEntry *entry = static_cast<LIMoSim::VehicleEntry*>(xml.parse(configuration));

        p_car = entry->toCar();
    }
    else
    {
        std::cout << "LIMoSimCar::setInitialPosition from NED" << std::endl;

        p_car = map->createCar();

        LIMoSim::PositionInfo info;

        std::string wayId = par("way").stringValue();
        int segmentIndex = par("segment");
        int laneIndex = par("lane");

        std::cout << wayId << "\t" << segmentIndex << "\t" << laneIndex << std::endl;


        LIMoSim::Way *way = nullptr;
        LIMoSim::Segment *segment = nullptr;
        do {
            do {
                do {
                    way = (wayId == "-1") ? map->getRandomWay() : map->getWay(wayId);
                } while (way == nullptr);
                segment = (segmentIndex == -1) ? map->getRandomSegment(way) : way->getSegment(segmentIndex);
            } while (segment == nullptr);
            info.lane = (laneIndex == -1) ? map->getRandomLane(segment) : segment->getLane(laneIndex);
        } while (info.lane == nullptr);
        info.laneOffset_m = par("offset").doubleValue();

        //
        LIMoSim::MobilityModel *strategicModel = strategicModel_omnet->createStrategicModel(p_car);
        p_car->setMobilityModel(strategicModel);


        //
        LIMoSim::FollowerModel *followerModel = followerModel_omnet->createFollowerModel(p_car);
        p_car->setFollowerModel(followerModel);



        LIMoSim::LaneChangeModel *laneChangeModel = new LIMoSim::MOBIL(p_car);
        p_car->setLaneChangeModel(laneChangeModel);


        p_car->setPositionInfo(info);
        p_car->setPosition(p_car->computeLanePosition(info.laneOffset_m, info.alignment_m, true));
    }

    p_car->initialize();

    updatePosition();

}

void LIMoSimCar::move()
{
    // we are not moving here, just updating the ui
    if(p_car)
        updatePosition();
}

void LIMoSimCar::orient()
{
    // we are not moving here, just updating the ui
    if(p_car)
        updateOrientation();
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

void LIMoSimCar::updateOrientation()
{
    if(p_car)
    {
        LIMoSim::Orientation orientation = p_car->getOrientation();
        lastOrientation.alpha = deg(orientation.getYaw());
        lastOrientation.beta = deg(orientation.getPitch());
        lastOrientation.gamma = deg(orientation.getRoll());
    }
}

}
