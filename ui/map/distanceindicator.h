#ifndef DISTANCEINDICATOR_H
#define DISTANCEINDICATOR_H

#include "mapelementui.h"

namespace LIMOSIM
{

class DistanceIndicator : public MapElementUi
{
    Q_OBJECT
public:
    DistanceIndicator(QQuickItem *_parent = 0);

    //
    void updatePosition();
    void handlePaintEvent();

    // EPS export
    QString toEps();
};


}


#endif // DISTANCEINDICATOR_H
