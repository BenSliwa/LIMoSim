#include "linestyle.h"
#include <QDebug>

namespace LIMoSim
{

LineStyle::LineStyle(const QString &_color, double _width, bool _close, bool _fill, int _style) :
    color(_color),
    width(_width),
    close(_close),
    fill(_fill),
    style(_style)
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void LineStyle::setStyleKey(const QString &_key)
{
    if(_key.contains("b"))
        color = "blue";
    else if(_key.contains("g"))
        color = "green";
    else if(_key.contains("r"))
        color = "red";
    else if(_key.contains("c"))
        color = "cyan";
    else if(_key.contains("m"))
        color = "magenta";
    else if(_key.contains("y"))
        color = "yellow";
    else if(_key.contains("k"))
        color = "black";
    else if(_key.contains("w"))
        color = "white";

    // parse the style
    if(_key.contains("-.."))
        style = Qt::DashDotDotLine;
    else if(_key.contains("-."))
        style = Qt::DashDotLine;
    else if(_key.contains("--"))
        style = Qt::DashLine;
    else if(_key.contains(":"))
        style = Qt::DotLine;
    else if(_key.contains("-"))
        style = Qt::SolidLine;
}

QPen LineStyle::toPen() const
{
    QPen pen;
    pen.setStyle((Qt::PenStyle)style);
    pen.setColor(color);
    pen.setWidthF(width);

    return pen;
}

}
