#include "legend.h"
#include "plot.h"

namespace LIMOSIM
{

Legend::Legend(QQuickItem *_parent) :
    UiElement(_parent)
{
    p_plot = qobject_cast<Plot*>(_parent);

    setWidth(80);
    setHeight(50);

    setAcceptedMouseButtons(Qt::AllButtons);

    setTitle("Vehicles");

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Legend::addGraph(Graph *_graph)
{
    if(!m_graphs.contains(_graph))
    {
        m_graphs << _graph;
        adjustHeight();
    }
}

void Legend::adjustHeight()
{
    double h = (m_graphs.size() + 0.5) * 15;
    if(m_title!="")
        h += 15;

    setHeight(h);
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Legend::setTitle(const QString &_title)
{
    m_title = _title;
    adjustHeight();
}

QString Legend::getTitle()
{
    return m_title;
}

/*************************************
 *              EPS EXPORT           *
 ************************************/

QString Legend::toEps(double _x, double _y)
{
    QString data;

    double x = 25 + _x;

    data += m_eps.drawRect(this->boundingRect(), QPointF(_x, _y), LineStyle("white", 1, false, true));


    TextStyle textStyle;
    textStyle.horizontalAlignment = Qt::AlignLeft;

    int offset = 0;
    if(m_title!="")
    {
        double y = height() - getY(0) + _y;

        TextStyle style;
        style.horizontalAlignment = Qt::AlignLeft;

        data += m_eps.drawText(m_title, QPointF(10 + _x, y), p_settings->legendHeaderStyle);
        offset = 1;
    }

    for(int i=0; i<m_graphs.size(); i++)
    {
        Graph *graph = m_graphs.at(i);

        double y = height() - getY(i+offset) + _y;

        data += m_eps.drawLine(QPointF(10 + _x, y), QPointF(10 + x, y), graph->getStyle());
        data += m_eps.drawText(graph->getId(), QPointF(20 + x, y), p_settings->legendItemStyle);
    }

    data += m_eps.drawRect(this->boundingRect(), QPointF(_x, _y), LineStyle());

    return data;
}


/*************************************
 *          ABSTRACT METHODS         *
 ************************************/

void Legend::handlePaintEvent()
{
    if(m_graphs.size()>0)
    {
        p_painter->fillRect(this->boundingRect(), "white");


        int offset = 0;
        if(m_title!="")
        {
            p_painter->setPen("black");
            drawText(m_title, QPointF(10, getY(0)), p_settings->legendHeaderStyle);

            offset = 1;
        }

        int s = m_graphs.size();
        for(int i=0; i<s; i++)
        {
            Graph *graph = m_graphs.at(i);

            double w = 25;
            double y = getY(i+offset);

            p_painter->setPen("black");
            drawText(graph->getId(), QPointF(20 + w, y), p_settings->legendItemStyle);

            p_painter->setPen(graph->getStyle().toPen());
            p_painter->drawLine(QPointF(10, y), QPointF(10 + w,y));
        }



        drawBox();
    }


}

void Legend::mousePressEvent(QMouseEvent *_event)
{
    qDebug() << "Legend::mousePressEvent";
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

double Legend::getY(int _index)
{
    int s = m_graphs.size() + 1;
    if(m_title!="")
        s++;

    return height() / s * (_index + 1);
}

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

}

