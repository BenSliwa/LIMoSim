#ifndef LIMOSIM_GRAPHPLOT_H
#define LIMOSIM_GRAPHPLOT_H

#include "plot.h"

namespace LIMoSim
{

class GraphPlot : public Plot
{
    Q_OBJECT
public:
    GraphPlot(QQuickItem *_parent = 0);
    virtual ~GraphPlot();

    void addPoint(const Point &_point, const QString &_graphId);

    // ui painting
    void handlePaintEvent();

    // EPS export
    QString toEps(const ExportConfig &_export);




private:
    QMap<QString,Graph*> m_graphs;


};

}

#endif // LIMOSIM_GRAPHPLOT_H
