#include "plot.h"
#include "figure.h"

namespace LIMoSim
{

Plot::Plot(QQuickItem *_parent) :
    UiElement(_parent),
    m_legend(new Legend(this)),
    m_drawBox(true),
    m_drawGrid(true)
{
    setXLimit(0, 300);
    setYLimit(0, 50);

    p_figure = qobject_cast<Figure*>(_parent);

    m_drawBox = p_settings->getValue("Figure", "Box").toInt();
    m_drawGrid = p_settings->getValue("Figure", "Grid").toInt();
}

Plot::~Plot()
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Plot::updateTicks(int _x, int _y)
{
    m_xTicks.clear();
    m_yTicks.clear();

    double xInc = width() / _x;
    double yInc = height() / _y;

    for(int x=0; x<_x+1; x++)
    {
        double value = x * (m_xLim.y-m_xLim.x) / _x + m_xLim.x;
        QString label = QString::number(value);


        if(m_xTickLabels.size()>0)
        {
            if(x>0 && x<_x)
                label = m_xTickLabels.at(x-1);
            else
                label = "";
        }


        Tick tick{label, x*xInc};

        m_xTicks << tick;
    }
    for(int y=0; y<_y+1; y++)
    {
        double value = y * (m_yLim.y-m_yLim.x) / _y + m_yLim.x;
        Tick tick{QString::number(value), y*yInc};

        m_yTicks << tick;
    }
}


void Plot::updateLegend()
{
    m_legend->setPosition(QPointF(width() - 25 - m_legend->width(), height() - 25 - m_legend->height()));
    m_legend->update();
}


void Plot::drawLineOnPlot(const QPointF &_from, const QPointF &_to, const LineStyle &_style)
{
    QPointF from = getPlotCoordinate(_from);
    QPointF to = getPlotCoordinate(_to);

    drawLine(from, to, _style);
}

void Plot::drawRectOnPlot(const QPointF &_origin, double _width, double _height, const LineStyle &_style)
{
    QPointF origin = getPlotCoordinate(_origin);
    origin = getCanvasPoint(Point{origin.x(), origin.y()});

    QPointF destination = getPlotCoordinate(QPointF(_origin.x()+_width, _origin.y()+_height));
    destination = getCanvasPoint(Point{destination.x(), destination.y()});

    double h = destination.y() - origin.y();
    double w = destination.x() - origin.x();

    QRectF rect(origin.x(), origin.y(), w, h);
    p_painter->setPen(_style.toPen());
    p_painter->drawRect(rect);
}

QPointF Plot::getPlotCoordinate(double _x, double _y)
{
    return getPlotCoordinate(QPointF(_x, _y));
}

QPointF Plot::getPlotCoordinate(const QPointF &_value) // map value range to ui range
{
    double x = map(_value.x(), m_xLim.x, m_xLim.y, 0, width());
    double y = map(_value.y(), m_yLim.x, m_yLim.y, 0, height());

    return QPointF(x, y);
}

QPointF Plot::getEpsCanvasCoordinate(double _x, double _y, const ExportConfig &_export)
{
    return getEpsCanvasCoordinate(QPointF(_x, _y), _export);
}

QPointF Plot::getEpsCanvasCoordinate(const QPointF &_value, const ExportConfig &_export)
{
    QPointF point(getPlotCoordinate(_value));
    point = mapToEpsCanvas(point, _export);

    return point;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Plot::setXLimit(double _min, double _max)
{
    m_xLim = {_min, _max};
}

void Plot::setYLimit(double _min, double _max)
{
    m_yLim = {_min, _max};
}

void Plot::setDrawGrid(bool _enabled)
{
    m_drawGrid = _enabled;
    update();
}

void Plot::setDrawBox(bool _enabled)
{
    m_drawBox = _enabled;
    update();
}


Legend* Plot::getLegend()
{
    return m_legend;
}

QList<Tick>& Plot::getXTicks()
{
    return m_xTicks;
}

QList<Tick>& Plot::getYTicks()
{
    return m_yTicks;
}

bool Plot::getDrawGrid()
{
    return m_drawGrid;
}

bool Plot::getDrawBox()
{
    return m_drawBox;
}


/*************************************
 *             UI PAINTING           *
 ************************************/

void Plot::drawGrid()
{
    LineStyle style("lightgray");

    if(m_xTicks.size()>1)
    {
        for(int x=1; x<m_xTicks.size()-1; x++)
        {
            Tick tick = m_xTicks.at(x);
            drawLine(QPointF(tick.canvas, m_yTicks.first().canvas), QPointF(tick.canvas, m_yTicks.last().canvas), style);
        }
    }

    if(m_yTicks.size()>1)
    {
        for(int y=1; y<m_yTicks.size()-1; y++)
        {
            Tick tick = m_yTicks.at(y);
            drawLine(QPointF(m_xTicks.first().canvas, tick.canvas), QPointF(m_xTicks.last().canvas, tick.canvas), style);
        }
    }

    p_figure->update();
}

void Plot::drawBox()
{
    UiElement::drawBox();

    LineStyle style;
    double markerSize = width() * 1 / 100;
    if(m_xTicks.size()>1)
    {
        for(int i=1; i<m_xTicks.size()-1; i++)
        {
            Tick tick = m_xTicks.at(i);

            QPointF top(tick.canvas, m_yTicks.first().canvas);
            QPointF bottom(tick.canvas, m_yTicks.last().canvas);

            drawLine(top, top + QPointF(0, markerSize), style);
            drawLine(bottom, bottom - QPointF(0, markerSize), style);
        }
    }

    if(m_yTicks.size()>1)
    {
        for(int i=1; i<m_yTicks.size()-1; i++)
        {
            Tick tick = m_yTicks.at(i);

            QPointF left(m_xTicks.first().canvas, tick.canvas);
            QPointF right(m_xTicks.last().canvas, tick.canvas);

            drawLine(left, left + QPointF(markerSize, 0), style);
            drawLine(right, right - QPointF(markerSize, 0), style);
        }
    }
}

/*************************************
 *              EPS EXPORT           *
 ************************************/


QString Plot::exportGrid(const ExportConfig &_export)
{
    QString data;

    LineStyle style("lightgray");

    if(m_xTicks.size()>1)
    {
        for(int i=1; i<m_xTicks.size()-1; i++)
        {
            Tick tick = m_xTicks.at(i);

            QPointF from(tick.canvas, m_yTicks.first().canvas);
            QPointF to(tick.canvas, m_yTicks.last().canvas);

            from = mapToEpsCanvas(from, _export);
            to = mapToEpsCanvas(to, _export);

            data += m_eps.drawLine(from, to, style);
        }
    }

    if(m_yTicks.size()>1)
    {
        for(int i=1; i<m_yTicks.size()-1; i++)
        {
            Tick tick = m_yTicks.at(i);

            QPointF from(m_xTicks.first().canvas, tick.canvas);
            QPointF to(m_xTicks.last().canvas, tick.canvas);

            from = mapToEpsCanvas(from, _export);
            to = mapToEpsCanvas(to, _export);

            data += m_eps.drawLine(from, to, style);
        }

    }

    return data;
}

QString Plot::exportBox(const ExportConfig &_export)
{
    QString data;

    data += m_eps.drawRect(QRectF(_export.offset.x(), _export.offset.y(), _export.width, _export.height), LineStyle("black", 1, true, false));

    LineStyle style;
    double markerSize = _export.width * 1 / 100;

    if(m_xTicks.size()>1)
    {
        for(int i=1; i<m_xTicks.size()-1; i++)
        {
            Tick tick = m_xTicks.at(i);

            QPointF bottom = mapToEpsCanvas(QPointF(tick.canvas, m_yTicks.first().canvas), _export);
            QPointF top = mapToEpsCanvas(QPointF(tick.canvas, m_yTicks.last().canvas), _export);

            data += m_eps.drawLine(bottom, bottom + QPointF(0, markerSize), style);
            data += m_eps.drawLine(top, top - QPointF(0, markerSize), style);
        }
    }

    if(m_yTicks.size()>1)
    {
        for(int i=1; i<m_yTicks.size()-1; i++)
        {
            Tick tick = m_yTicks.at(i);

            QPointF left = mapToEpsCanvas(QPointF(m_xTicks.first().canvas, tick.canvas), _export);
            QPointF right = mapToEpsCanvas(QPointF(m_xTicks.last().canvas, tick.canvas), _export);

            data += m_eps.drawLine(left, left + QPointF(markerSize, 0), style);
            data += m_eps.drawLine(right, right - QPointF(markerSize, 0), style);
        }
    }

    return data;
}


QString Plot::exportPlotLine(double _fromX, double _fromY, double _toX, double _toY, const LineStyle &_style, const ExportConfig &_export)
{
    return exportPlotLine(QPointF(_fromX, _fromY), QPointF(_toX, _toY), _style, _export);
}

QString Plot::exportPlotLine(const QPointF &_from, const QPointF &_to, const LineStyle &_style, const ExportConfig &_export)
{
    QPointF from = getPlotCoordinate(_from);
    QPointF to = getPlotCoordinate(_to);

    return exportCanvasLine(from, to, _style, _export);
}

QString Plot::exportCanvasLine(const QPointF &_from, const QPointF &_to, const LineStyle &_style, const ExportConfig &_export)
{
    QPointF from = mapToEpsCanvas(_from, _export);
    QPointF to = mapToEpsCanvas(_to, _export);

    return m_eps.drawLine(from, to, _style);
}




}
