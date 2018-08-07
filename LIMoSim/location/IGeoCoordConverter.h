#ifndef LIMOSIM_IGEOCOORDCONVERTER_H
#define LIMOSIM_IGEOCOORDCONVERTER_H

#include "position.h"

namespace LIMoSim
{

class IGeoCoordConverter
{
public:
    virtual ~IGeoCoordConverter() {}

    virtual void setOrigin(const Position &_origin) = 0;
    virtual Vector3d getOffset(const Position &_node) const = 0;
};

}

#endif // LIMOSIM_IGEOCOORDCONVERTER_H
