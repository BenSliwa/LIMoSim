#include "textstyle.h"

namespace LIMoSim
{

TextStyle::TextStyle(const QString &_font, int _fontSize, const QString &_color, int _rotation, int _horizontalAlignment, int _verticalAlignment) :
    font(_font),
    fontSize(_fontSize),
    color(_color),
    rotation(_rotation),
    horizontalAlignment(_horizontalAlignment),
    verticalAlignment(_verticalAlignment)
{

}

}
