#ifndef PLOTOVERLAY_H
#define PLOTOVERLAY_H

#include "uielement.h"

namespace LIMoSim
{

class PlotOverlay : public UiElement
{
    Q_OBJECT
public:
    PlotOverlay(QQuickItem *_parent = 0);

    // ui painting
    void handlePaintEvent();


};

}



#endif // PLOTOVERLAY_H
