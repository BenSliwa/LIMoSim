#ifndef LIMOSIM_FIGURE_H
#define LIMOSIM_FIGURE_H

#include "uielement.h"
#include "plot.h"
#include "plotoverlay.h"
#include "ui/sim/imagebutton.h"

namespace LIMoSim
{

class Figure : public UiElement
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle WRITE setTitle)
    Q_PROPERTY(QString xLabel READ getXLabel WRITE setXLabel)
    Q_PROPERTY(QString yLabel READ getYLabel WRITE setYLabel)
    Q_PROPERTY(Plot* plot READ getPlot)
public:
    Figure(QQuickItem *_parent = 0);

    void hoverEnterEvent(QHoverEvent *_event);
    void hoverLeaveEvent(QHoverEvent *_event);

    // property accessors
    void setPlot(Plot *_plot);
    void setTitle(const QString &_text);
    void setXLabel(const QString &_text);
    void setYLabel(const QString &_text);

    Plot *getPlot();
    QString getTitle();
    QString getXLabel();
    QString getYLabel();

    // EPS export
    QString toEps(const ExportConfig &_export);
    QString exportLabels(const ExportConfig &_export);
    QString exportTicks(const ExportConfig &_export);


protected:
    void handlePaintEvent();


private:
    void drawLabels();
    void drawTicks();

private slots:
    void onScaleChanged();
    void onOverlayButtonStateChanged(int _state);

private:
    Plot *m_plot;
    PlotOverlay *m_overlay;

    QString m_title;
    QString m_xLabel;
    QString m_yLabel;

    Margin m_margin;
};

}

#endif // LIMOSIM_FIGURE_H
