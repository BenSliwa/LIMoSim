#ifndef EXPORTCONFIG_H
#define EXPORTCONFIG_H

#include <QPointF>
#include <QTextStream>

namespace SCSIM
{


class ExportConfig
{
public:
    ExportConfig();

    QPointF offset;
    double width;
    double height;

    QTextStream stream;
};

}


#endif // EXPORTCONFIG_H
