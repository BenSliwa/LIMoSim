/*
 * StrategicModel.h
 *
 *  Created on: May 12, 2017
 *      Author: sliwa
 */

#ifndef INET_STRATEGICMODEL_H_
#define INET_STRATEGICMODEL_H_

#include <omnetpp.h>
#include "core/mobility/mobilitymodels.h"

using namespace omnetpp;

class LIMoSim::Car;

class StrategicModel : public cSimpleModule
{
public:
    virtual LIMoSim::MobilityModel* createStrategicModel(LIMoSim::Car *_car) = 0;
};

#endif /* INET__STRATEGICMODEL_H_ */
