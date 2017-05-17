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

#include "IDM.h"
#include "core/mobility/followerModel/idm.h"

namespace inet {

Define_Module(IDM);

LIMoSim::FollowerModel* IDM::createFollowerModel(LIMoSim::Car *_car)
{
    LIMoSim::IDM *idm = new LIMoSim::IDM(_car);

    idm->setSafeTimeheadway(par("safeHeadway").doubleValue());
    idm->setMaxAcceleration( par("maxAcceleration").doubleValue());
    idm->setMaxDeceleration(par("maxDeceleration").doubleValue());
    idm->setAccelerationExponent(par("accelerationExponent").doubleValue());
    idm->setMinDistance(par("minDistance").doubleValue());

    return idm;
}

} //namespace
