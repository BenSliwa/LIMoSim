#ifndef LIMOSIM_PLOT_H
#define LIMOSIM_PLOT_H

#include "uielement.h"
#include "legend.h"


/*
 *
 * mouse enter: show context overlay: grid, box, save
 *
 *
 */

namespace LIMOSIM
{

class Figure;

struct Tick
{
    QString label;
    double canvas;
};

class Plot : public UiElement
{
    Q_OBJECT
public:
    Plot(QQuickItem *_parent = 0);
    virtual ~Plot();

    void updateTicks(int _x, int _y);
    void updateLegend();

    void drawLineOnPlot(const QPointF &_from, const QPointF &_to, const LineStyle &_style);
    void drawRectOnPlot(const QPointF &_origin, double _width, double _height, const LineStyle &_style);


    QPointF getPlotCoordinate(double _x, double _y);
    QPointF getPlotCoordinate(const QPointF &_value);
    QPointF getEpsCanvasCoordinate(double _x, double _y, const ExportConfig &_export);
    QPointF getEpsCanvasCoordinate(const QPointF &_value, const ExportConfig &_export);

    // property accessors
    void setXLimit(double _min, double _max);
    void setYLimit(double _min, double _max);
    void setDrawGrid(bool _enabled);
    void setDrawBox(bool _enabled);

    Legend* getLegend();
    QList<Tick>& getXTicks();
    QList<Tick>& getYTicks();
    bool getDrawGrid();
    bool getDrawBox();


    // ui painting
    void drawGrid();
    void drawBox();

    // EPS export
    virtual QString toEps(const ExportConfig &_export) = 0;
    QString exportGrid(const ExportConfig &_export);
    QString exportBox(const ExportConfig &_export);
    QString exportPlotLine(double _fromX, double _fromY, double _toX, double _toY, const LineStyle &_style, const ExportConfig &_export);
    QString exportPlotLine(const QPointF &_from, const QPointF &_to, const LineStyle &_style, const ExportConfig &_export);

    QString exportCanvasLine(const QPointF &_from, const QPointF &_to, const LineStyle &_style, const ExportConfig &_export);

protected:
    Figure *p_figure;
    Legend *m_legend;

    Point m_xLim;
    Point m_yLim;

    QList<Tick> m_xTicks;
    QList<Tick> m_yTicks;
    QStringList m_xTickLabels;

    bool m_drawBox;
    bool m_drawGrid;
};

}

#endif // LIMOSIM_PLOT_H
