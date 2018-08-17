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

#ifndef __INET_EVENTSCHEDULER_H_
#define __INET_EVENTSCHEDULER_H_

#include <omnetpp.h>

#include "inet/common/geometry/common/GeographicCoordinateSystem.h"

#include "LIMoSim/location/IGeoCoordConverter.h"
#include "LIMoSim/sim/event.h"
#include "LIMoSim/sim/eventscheduler.h"

using namespace omnetpp;

namespace inet
{

class LIMoSimController : public cSimpleModule, public LIMoSim::EventScheduler, public LIMoSim::IGeoCoordConverter
{
public:
    LIMoSimController();
    ~LIMoSimController();

    virtual void initialize() override;

    void scheduleEvent(LIMoSim::Event *_event) override;
    void cancelEvent(LIMoSim::Event *_event) override;
    void deleteEvent(LIMoSim::Event *_event) override;

    cMessage* getMessageForEvent(LIMoSim::Event *_event);

    // IGeoCoordConverter:
    virtual void setOrigin(const LIMoSim::Position &_origin) override;
    virtual LIMoSim::Vector3d getOffset(const LIMoSim::Position &_node) const override;

protected:
    virtual void handleMessage(cMessage *_message) override;

private:
    std::map<cMessage*, LIMoSim::Event*> m_events;
    IGeographicCoordinateSystem *m_geographicCoordinateSystemModule = nullptr;

};

} //namespace

#endif
