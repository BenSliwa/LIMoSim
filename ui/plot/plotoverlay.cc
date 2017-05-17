#include "plotoverlay.h"

namespace LIMOSIM
{

PlotOverlay::PlotOverlay(QQuickItem *_parent) :
    UiElement(_parent)
{
    double h = 30;

    setWidth(p_parent->width()-50);
    setHeight(h);
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/


/*************************************
 *             UI PAINTING           *
 ************************************/

void PlotOverlay::handlePaintEvent()
{
    p_painter->fillRect(this->boundingRect(), "#1c1c1c");
}

}

