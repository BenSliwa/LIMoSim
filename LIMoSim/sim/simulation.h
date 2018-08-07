#ifndef LIMOSIM_SIMULATION_H
#define LIMOSIM_SIMULATION_H

#include "LIMoSim/map/map.h"
#include "LIMoSim/map/node.h"
#include "LIMoSim/map/way.h"
#include "LIMoSim/location/IGeoCoordConverter.h"

#include "eventscheduler.h"



namespace LIMoSim
{

class Simulation
{
public:
    Simulation(EventScheduler *_scheduler = 0);


    static Simulation* getInstance(EventScheduler *_scheduler = 0);
    static bool hasInstance();

    void registerEventHandler(EventHandler *_eventHandler);
    void deregisterEventHandler(EventHandler *_eventHandler);

    void load(const std::string &_map, const std::string &_vehicles, IGeoCoordConverter &geoCoordConverter);

    void createCars(int _number);

    // property accessors
    void setEventScheduler(EventScheduler *_scheduler);

    EventScheduler* getEventScheduler();


private:
    EventScheduler *m_eventScheduler;

    std::vector<EventHandler*> m_eventHandler;





};

}

#endif // LIMOSIM_SIMULATION_H
