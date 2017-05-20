#include "graphplot.h"
#include "LIMoSim/sim/simulation.h"
#include "ui/qteventscheduler.h"

namespace LIMoSim
{

GraphPlot::GraphPlot(QQuickItem *_parent) :
    Plot(_parent)
{

}

GraphPlot::~GraphPlot()
{
    QMap<QString,Graph*>::iterator it;
    for(it=m_graphs.begin(); it!=m_graphs.end(); it++)
    {
        Graph *graph = it.value();
        delete graph;
    }
    m_graphs.clear();
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void GraphPlot::addPoint(const Point &_point, const QString &_graphId)
{
    QStringList colors = QStringList() << "b" << "g" << "r" << "c" << "m" << "y" << "k" << "w";

    Graph *graph = 0;
    if(m_graphs.contains(_graphId))
        graph = m_graphs.value(_graphId);
    else
    {
        QString style = colors.at(m_graphs.size());
        style += QString::fromStdString(p_settings->getValue("Figure", "Style").toString());

        graph = new Graph();
        graph->setId(_graphId);

        LineStyle lineStyle;
        lineStyle.setStyleKey(style);
        lineStyle.width = p_settings->getValue("Figure", "LineWidth").toDouble();
        graph->setStyle(lineStyle);

        m_graphs.insert(_graphId, graph);
        m_legend->addGraph(graph);

        updateLegend();


        updateTicks(5,5);
    }

    graph->add(_point);
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/



/*************************************
 *             UI PAINTING           *
 ************************************/

void GraphPlot::handlePaintEvent()
{
    p_painter->fillRect(this->boundingRect(), QColor("white"));

    if(m_drawGrid)
        drawGrid();


    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());

    double time_s = scheduler->getSimTime();
    QMap<QString,Graph*>::iterator it;


    double minX = std::numeric_limits<double>::max();
    for(it=m_graphs.begin(); it!=m_graphs.end(); it++)
    {
        Graph *graph = it.value();
        Point range = graph->getXRange();

        if(range.x<minX)
            minX = range.x;
    }


    for(it=m_graphs.begin(); it!=m_graphs.end(); it++)
    {
        Graph *graph = it.value();
        graph->adjustToRange(time_s, m_xLim.y-m_xLim.x);
        QList<Point> &points = graph->getPoints();

        QPainterPath painterPath;
        QPolygonF polygon;

        for(int i=0; i<points.size(); i++)
        {
            Point to = points.at(i);
            to.x -= minX;
            to.x = map(to.x, m_xLim.x, m_xLim.y, 0, width());
            to.y = map(to.y, m_yLim.x, m_yLim.y, 0, height());

            polygon << getCanvasPoint(to).toPoint();

        }

        painterPath.addPolygon(polygon);
        p_painter->setPen(graph->getStyle().toPen());
        p_painter->drawPath(painterPath);
    }




    if(m_drawBox)
        drawBox();
}




/*************************************
 *              EPS EXPORT           *
 ************************************/

QString GraphPlot::toEps(const ExportConfig &_export)
{
    QString data;


    data += m_eps.drawRect(QRectF(_export.offset.x(), _export.offset.y(), _export.width, _export.height), LineStyle("white", 1, true, true));

    if(m_drawGrid)
        data += exportGrid(_export);

    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());

    double time_s = scheduler->getSimTime();
    QMap<QString,Graph*>::iterator it;

    double minX = std::numeric_limits<double>::max();
    for(it=m_graphs.begin(); it!=m_graphs.end(); it++)
    {
        Graph *graph = it.value();
        Point range = graph->getXRange();

        if(range.x<minX)
            minX = range.x;
    }


    for(it=m_graphs.begin(); it!=m_graphs.end(); it++)
    {
        Graph *graph = it.value();
        graph->adjustToRange(time_s, m_xLim.y-m_xLim.x);
        QList<Point> &points = graph->getPoints();

        QPainterPath painterPath;
        QPolygonF polygon;

        for(int i=0; i<points.size(); i++)
        {
            Point to = points.at(i);
            to.x -= minX;
            polygon << getEpsCanvasCoordinate(to.x, to.y, _export);
        }

        painterPath.addPolygon(polygon);


        LineStyle style = graph->getStyle();
        style.close = false;
        style.fill = false;


        data += m_eps.drawPath(painterPath, style);

    }


    if(m_drawBox)
        data += exportBox(_export);


    // the legend is not scaled, only the position is updated
    data += m_legend->toEps(_export.width - 25 - m_legend->width() + _export.offset.x(), 25  + _export.offset.y());

    return data;
}





}
