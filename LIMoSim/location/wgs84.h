#ifndef LIMOSIM_WGS84_H
#define LIMOSIM_WGS84_H

#include "IGeoCoordConverter.h"
#include "position.h"
#include "math_.h"

namespace LIMoSim
{

class WGS84 : public IGeoCoordConverter
{
public:
    WGS84();

    double getDistance(const Position &_from, const Position &_to) const;

    virtual void setOrigin(const Position &_origin) override { m_origin = _origin; }
    virtual Vector3d getOffset(const Position &_node) const override;
private:
    Position m_origin;
};

}

#endif // LIMOSIM_WGS84_H
