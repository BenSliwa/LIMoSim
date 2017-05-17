#ifndef LIMOSIM_BOXPLOT_H
#define LIMOSIM_BOXPLOT_H

#include "plot.h"
#include "box.h"

namespace LIMOSIM
{

class BoxPlot : public Plot
{
    Q_OBJECT
public:
    BoxPlot(QQuickItem *_parent = 0);
    ~BoxPlot();

    void addValue(double _value, const QString &_boxId);

    // ui painting
    void handlePaintEvent();
    void drawBox(Box *_box, int _index);
    void drawOutlier(double _x, double _y);

    // EPS export
    QString toEps(const ExportConfig &_export);
    QString exportBoxplot(Box *_box, int _index, const ExportConfig &_export);




private:
    QMap<QString,Box*> m_data;
};

}

#endif // LIMOSIM_BOXPLOT_H
