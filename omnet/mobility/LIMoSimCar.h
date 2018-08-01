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

#ifndef __INET_LIMoSimCAR_H_
#define __INET_LIMoSimCAR_H_


#include "inet/mobility/base/MovingMobilityBase.h"


#include "LIMoSim/vehicles/car.h"

namespace inet {


class INET_API LIMoSimCar : public MovingMobilityBase
{

public:
    LIMoSimCar();
    ~LIMoSimCar();

protected:
    virtual void initialize(int _stage) override;
    virtual void setInitialPosition() override;
    virtual void move() override;
    virtual void orient() override;

    void updatePosition();
    void updateOrientation();

private:
    LIMoSim::Car *p_car = 0;
};

}

#endif
