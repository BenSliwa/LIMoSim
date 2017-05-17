#ifndef LIMOSIM_INFOBOX_H
#define LIMOSIM_INFOBOX_H

#include "ui/plot/uielement.h"

namespace LIMOSIM
{

class InfoBox : public UiElement
{
    Q_OBJECT
public:
    InfoBox(QQuickItem *_parent = 0);

    // ui painting
    void handlePaintEvent();
};



}

#endif // LIMOSIM_INFOBOX_H
