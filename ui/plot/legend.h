#ifndef LIMOSIM_LEGEND_H
#define LIMOSIM_LEGEND_H

#include "uielement.h"
#include "graph.h"


namespace LIMOSIM
{

class Plot;

class Legend : public UiElement
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle WRITE setTitle)
public:
    Legend(QQuickItem *_parent = 0);

    void addGraph(Graph *_graph);
    void adjustHeight();

    // property accessors
    void setTitle(const QString &_title);
    QString getTitle();

    // EPS export
    QString toEps(double _x, double _y);

protected:
    void handlePaintEvent();
    void mousePressEvent(QMouseEvent *_event);

private:
    double getY(int _index);

private:
    Plot *p_plot;
    QList<Graph*> m_graphs;

    QString m_title;
};

}

#endif // LIMOSIM_LEGEND_H
