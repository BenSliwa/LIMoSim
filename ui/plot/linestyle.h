#ifndef LIMOSIM_LINESTYLE_H
#define LIMOSIM_LINESTYLE_H

#include <QString>
#include <QPen>

namespace LIMoSim
{

class LineStyle
{
public:
    LineStyle(const QString &_color="black", double _width=1, bool _close=false, bool _fill=false, int _style=Qt::SolidLine);


    void setStyleKey(const QString &_key);

    QPen toPen() const;

public:
    QString color;
    double width;
    bool close;
    bool fill;
    int style;
};

}

#endif // LIMOSIM_LINESTYLE_H
