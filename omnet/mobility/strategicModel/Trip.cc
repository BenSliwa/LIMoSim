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

#include "Trip.h"
#include "LIMoSim/map/map.h"
#include "LIMoSim/map/osm/parser.h"

namespace inet {

Define_Module(Trip);

LIMoSim::MobilityModel* Trip::createStrategicModel(LIMoSim::Car *_car)
{
    LIMoSim::Trip *model = new LIMoSim::Trip(_car);

    //std::cout << "Trip::createStrategicModel\t" << strategicModel_omnet->par("trip").stringValue() << std::endl;

    std::cout << "Trip::createStrategicModel" << std::endl;

    LIMoSim::Map *map = LIMoSim::Map::getInstance();
    std::vector<std::string> nodeIds = LIMoSim::Parser::split(par("trip").stringValue(), ",");
    for(unsigned int i=0; i<nodeIds.size(); i++)
    {
        LIMoSim::Node *node = map->getNode(nodeIds.at(i));
        model->addTripEntry(node);
    }

    return model;
}

} //namespace
