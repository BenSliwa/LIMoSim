#ifndef LIMOSIM_VEHICLEPARSER_H
#define LIMOSIM_VEHICLEPARSER_H

#include "parser.h"
#include "LIMoSim/map/map.h"

namespace LIMoSim
{

class VehicleParser : public Parser
{
public:
    VehicleParser();

    void loadScenario(DOMElement *_dom);

private:
    void parseCar(DOMElement *_dom);
    void parseCarTag(const std::string &_key, const Variant &_value, Car *_car);
    std::vector<Node*> parsePath(DOMElement *_dom);

private:
    Map *p_map;
};

}

#endif // VEHICLEPARSER_H
