#include "figure.h"
#include "graphplot.h"
#include "boxplot.h"

namespace LIMOSIM
{

Figure::Figure(QQuickItem *_parent) :
    UiElement(_parent),
    m_plot(0),
    m_overlay(0)
{
    m_margin = {40, 10, 15, 24};

    setZ(300);

    setAcceptHoverEvents(true);
    connect(_parent, SIGNAL(widthChanged()), this, SLOT(onScaleChanged()));
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Figure::hoverEnterEvent(QHoverEvent *_event)
{
    if(!m_overlay)
    {
        m_overlay = new PlotOverlay(this);

        double s = 30;
        int i = 0;

        ImageButton *gridButton = new ImageButton(m_overlay);
        gridButton->setText("Grid");
        gridButton->setId("GridButton");
        gridButton->setState((m_plot->getDrawGrid()==true) ? BUTTON_STATE::ON : BUTTON_STATE::OFF);
        connect(gridButton, SIGNAL(stateChanged(int)), this, SLOT(onOverlayButtonStateChanged(int)));
        gridButton->setPosition(QPointF(s*i++, 0));

        ImageButton *boxButton = new ImageButton(m_overlay);
        boxButton->setText("Box");
        boxButton->setId("BoxButton");
        boxButton->setState((m_plot->getDrawBox()==true) ? BUTTON_STATE::ON : BUTTON_STATE::OFF);
        connect(boxButton, SIGNAL(stateChanged(int)), this, SLOT(onOverlayButtonStateChanged(int)));
        boxButton->setPosition(QPointF(s*i++, 0));

        ImageButton *saveButton = new ImageButton(m_overlay);
        saveButton->setText("Save");
        saveButton->setId("SaveButton");
        connect(saveButton, SIGNAL(stateChanged(int)), this, SLOT(onOverlayButtonStateChanged(int)));
        saveButton->setPosition(QPointF(s*i++, 0));

        m_overlay->setWidth(s*i);
    }
}

void Figure::hoverLeaveEvent(QHoverEvent *_event)
{
    if(m_overlay)
    {
        m_overlay->deleteLater();
        m_overlay = 0;
    }
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Figure::setPlot(Plot *_plot)
{
    m_plot = _plot;
}

void Figure::setTitle(const QString &_text)
{
    m_title = _text;
}

void Figure::setXLabel(const QString &_text)
{
    m_xLabel = _text;
}

void Figure::setYLabel(const QString &_text)
{
    m_yLabel = _text;
}

Plot* Figure::getPlot()
{
    return m_plot;
}

QString Figure::getTitle()
{
    return m_title;
}

QString Figure::getXLabel()
{
    return m_xLabel;
}

QString Figure::getYLabel()
{
    return m_yLabel;
}


/*************************************
 *              EPS EXPORT           *
 ************************************/


QString Figure::toEps(const ExportConfig &_export)
{
    QString data = readFile(":/EpsHeader.txt").join("\n");
    data.replace("$BOX_WIDTH$", QString::number(_export.width));
    data.replace("$BOX_HEIGHT$", QString::number(_export.height));

    //data += rect2Eps(this->boundingRect(), QPointF(0, 0), LineStyle("darkgray", 1, true, true));


    qDebug() << "EXPORT" << _export.width << _export.height;

    double w = _export.width-m_margin.left-m_margin.right;
    double h = _export.height-m_margin.top-m_margin.bottom;

    data += m_plot->toEps(ExportConfig{QPointF(m_margin.left, m_margin.bottom), w, h});

    data += exportLabels(_export);
    data += exportTicks(ExportConfig{QPointF(0,0), w, h});



    data += "\n" + readFile(":/EpsFooter.txt").join("\n");



    return data;
}

QString Figure::exportLabels(const ExportConfig &_export)
{
    QString data;

    double x = map(width()/2, width(), _export.width);
    QPointF point(x, m_margin.bottom*1/4);


    TextStyle style = p_settings->labelStyle;
    data += m_eps.drawText(m_xLabel, point, style);

    style.rotation = 90;
    double y = map(height()/2, height(), _export.height);

    point = QPointF(m_margin.left*1/4, y);
    data += m_eps.drawText(m_yLabel, point, style);

    return data;
}

QString Figure::exportTicks(const ExportConfig &_export)
{
    QString data;

    TextStyle style = p_settings->tickStyle;

    QList<Tick>& xTicks = m_plot->getXTicks();
    for(int i=0; i<xTicks.size(); i++)
    {
        Tick tick = xTicks.at(i);
        double x = tick.canvas ;
        QPointF point(map(x, m_plot->width(), _export.width), 0);
        QPointF offset(m_margin.left, m_margin.bottom*3/4);

        data += m_eps.drawText(tick.label, point + offset, style);
    }

    QList<Tick>& yTicks = m_plot->getYTicks();
    for(int i=0; i<yTicks.size(); i++)
    {
        Tick tick = yTicks.at(i);
        double y = tick.canvas;
        QPointF point(0, map(y, m_plot->height(), _export.height));
        QPointF offset(m_margin.left*3/4, m_margin.bottom);

        data += m_eps.drawText(tick.label, point + offset, style);
    }

    return data;
}


/*************************************
 *          ABSTRACT METHODS         *
 ************************************/

void Figure::handlePaintEvent()
{
    p_painter->fillRect(this->boundingRect(), QColor("darkgray"));

    m_plot->setWidth(width()-m_margin.left-m_margin.right);
    m_plot->setHeight(height()-m_margin.top-m_margin.bottom);
    m_plot->setPosition(QPointF(m_margin.left, m_margin.top));


    drawLabels();
    drawTicks();
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void Figure::drawLabels()
{
    TextStyle style = p_settings->labelStyle;
    drawText(m_title, QPointF(width()/2, 8), style);
    drawText(m_xLabel, QPointF(width()/2, height()-(m_margin.bottom*1/4)), style);

    style.rotation = -90;
    drawText(m_yLabel, QPointF(m_margin.left*1/4, height()/2), style);
}

void Figure::drawTicks()
{
    TextStyle style = p_settings->tickStyle;
    QList<Tick>& xTicks = m_plot->getXTicks();
    for(int i=0; i<xTicks.size(); i++)
    {
        Tick tick = xTicks.at(i);
        double x = tick.canvas + m_margin.left;

        drawText(tick.label, QPointF(x, height() - (m_margin.bottom*3/4)), style);
    }

    QList<Tick>& yTicks = m_plot->getYTicks();
    for(int i=0; i<yTicks.size(); i++)
    {
        Tick tick = yTicks.at(i);
        double y = tick.canvas + m_margin.bottom;

        drawText(tick.label, QPointF(m_margin.left*3/4, height() - y), style);
    }
}

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void Figure::onScaleChanged()
{
    setWidth(p_parent->width());
    setPosition(QPointF(0, p_parent->height() - height()));

    m_plot->updateLegend();
}

void Figure::onOverlayButtonStateChanged(int _state)
{
    ImageButton *button = qobject_cast<ImageButton*>(QObject::sender());
    if(button)
    {
        QString id = button->getId();
        if(id=="GridButton")
        {
            bool enabled = (_state==BUTTON_STATE::ON) ? true : false;
            m_plot->setDrawGrid(enabled);
        }
        else if(id=="BoxButton")
        {
            bool enabled = (_state==BUTTON_STATE::ON) ? true : false;
            m_plot->setDrawBox(enabled);
        }
        else if(id=="SaveButton")
        {
            double w = width();
            double h = height();

            if(p_settings->hasValue("Figure", "Width"))
                w = p_settings->getValue("Figure", "Width").toDouble();
            if(p_settings->hasValue("Figure", "Height"))
                h = p_settings->getValue("Figure", "Height").toDouble();

            writeToFile(toEps(ExportConfig{QPointF(0,0), w, h}), "figure.eps");
        }

    }
}



}
