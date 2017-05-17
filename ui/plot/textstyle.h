#ifndef LIMOSIM_TEXTSTYLE_H
#define LIMOSIM_TEXTSTYLE_H

#include <QString>

namespace LIMoSim
{

class TextStyle
{
public:
    TextStyle(const QString &_font="Arial", int _fontSize=10, const QString &_color="black", int _rotation=0, int _horizontalAlignment=Qt::AlignHCenter, int _verticalAlignment=Qt::AlignVCenter);

    /*
    void alignLeft();
    void alignRight();
    void alignHCenter();
    void alignTop();
    void alignBottom();
    void alignVCenter();*/

public:
    QString font;
    double fontSize;
    QString color;
    double rotation;
    int horizontalAlignment;
    int verticalAlignment;
};

}
#endif // LIMOSIM_TEXTSTYLE_H
