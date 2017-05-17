#ifndef SCSIM_CARENTRY_H
#define SCSIM_CARENTRY_H

#include "osmentry.h"
#include "core/vehicles/car.h"

namespace SCSIM
{

class CarEntry : public OSMEntry
{
public:
    CarEntry(OSMDocument *_parent = 0);

    static CarEntry fromXML(DOMElement *_entry, OSMDocument *_parent = 0);
    static CarEntry fromCar(Car *_car, OSMDocument *_parent = 0);

    Car* toCar() const;
    DOMElement* toXML() const;

private:
    std::string id;
    std::string way;
    int segment;                            // TODO: car entry shoud inherit directly from DOMElement
    int lane;
    double offset_m;

    double speed_mps;
    double maxSpeed_mps;

    std::string currentNode;
    std::vector<std::string> path;

    std::string mobilityModel;
    std::vector<std::string> trip;

    std::string followerModel;
    std::string laneChangeModel;
};

}

#endif // SCSIM_CARENTRY_H
