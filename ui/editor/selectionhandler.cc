#include "selectionhandler.h"
#include "ui/map/carui.h"
#include <QGuiApplication>

#include "ui/map/mapui.h"
#include "ui/map/pathlayer.h"
#include "ui/plot/graphplot.h"
#include "ui/plot/boxplot.h"
#include"ui/qteventscheduler.h"

#include "ui/map/viewport.h"


namespace LIMoSim
{

SelectionHandler *selectionHandlerInstance = 0;

SelectionHandler::SelectionHandler(QObject *_parent) :
    QObject(_parent),
    m_figure(0),
    m_boxPlot(0)
{
    selectionHandlerInstance = this;

    m_updateInterval_s = 250.0/1000.0;
    m_updateTimer = new Event(m_updateInterval_s, this, "SelectionHandler::updateTimer");
    scheduleEvent(m_updateTimer);

}

SelectionHandler::~SelectionHandler()
{
    deleteEvent(m_updateTimer);
}

SelectionHandler* SelectionHandler::getInstance()
{
    if(!selectionHandlerInstance)
        selectionHandlerInstance = new SelectionHandler;

    return selectionHandlerInstance;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


void SelectionHandler::selectCar(CarUi *_car)
{
    bool ctrlPressed = (QGuiApplication::keyboardModifiers()==Qt::ControlModifier);

    if(!ctrlPressed)
        clearSelection();

    // create a new figure
    if(m_selectedCars.size()==0)
    {
        MapUi *map = MapUi::getInstance();
        ViewPort *viewPort = ViewPort::getInstance();

        m_figure = new Figure(viewPort);
        GraphPlot *graphPlot = new GraphPlot(m_figure);
        m_figure->setPlot(graphPlot);
        m_figure->setHeight(150);
        m_figure->setWidth(viewPort->width());
        m_figure->setPosition(QPointF(0, viewPort->height()-150));
        m_figure->setXLabel("Time [s]");
        m_figure->setYLabel("Speed [km/h]");


        m_boxPlot = new Figure(viewPort);
        BoxPlot *boxplot = new BoxPlot(m_boxPlot);

        m_boxPlot->setXLabel("Vehicle");
        m_boxPlot->setYLabel("Speed [km/h]");
        m_boxPlot->setPlot(boxplot);

        boxplot->setXLimit(0, 10);
        boxplot->setYLimit(0, 50);
        boxplot->setYLimit(-10, 60);

        //



        m_boxPlot->setHeight(150);
        m_boxPlot->setWidth(300);
    }

    if(!m_selectedCars.contains(_car))
        m_selectedCars << _car;

    // select the object



    // update the ui

    MapUi::getInstance()->getPathLayer()->update();
}

void SelectionHandler::clearSelection()
{
    // deselect all objbts
    for(int i=0; i<m_selectedCars.size(); i++)
    {
        CarUi *car = m_selectedCars.at(i);
      //  car->softSelection();
    }


    m_selectedCars.clear();

    //
    if(m_figure)
    {
        m_figure->deleteLater();
        m_figure = 0;
    }

    if(m_boxPlot)
    {
        m_boxPlot->deleteLater();
        m_boxPlot = 0;
    }

    // update the ui
    MapUi::getInstance()->getPathLayer()->update();
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

QList<CarUi*>& SelectionHandler::getSelectedCars()
{
    return m_selectedCars;
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void SelectionHandler::handleEvent(Event *_event)
{
    if(_event==m_updateTimer)
    {
        onFigureUpdateTimeout();

        scheduleEvent(m_updateTimer, m_updateInterval_s);
    }
}

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void SelectionHandler::onFigureUpdateTimeout()
{
    if(m_boxPlot)
    {
        BoxPlot *boxPlot = qobject_cast<BoxPlot*>(m_boxPlot->getPlot());

        for(int i=0; i<m_selectedCars.size(); i++)
        {
            CarUi *ui = m_selectedCars.at(i);
            Car *car = ui->getCar();



            double speed_kmh = car->getSpeed() * 3.6;
            /*
            speed_kmh = car->getDesiredSpeed() * 3.6;
            speed_kmh = car->getAcceleration();
            plot->setYLimit(-3, 1);*/

            boxPlot->addValue(speed_kmh, QString::fromStdString(car->getName()));

        }

        boxPlot->update();
    }

    if(m_figure)
    {
        GraphPlot *plot = qobject_cast<GraphPlot*>(m_figure->getPlot());

        QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());

        double time_s = scheduler->getSimTime();
        for(int i=0; i<m_selectedCars.size(); i++)
        {
            CarUi *ui = m_selectedCars.at(i);
            Car *car = ui->getCar();



            double speed_kmh = car->getSpeed() * 3.6;
            /*
            speed_kmh = car->getDesiredSpeed() * 3.6;
            speed_kmh = car->getAcceleration();
            plot->setYLimit(-3, 1);*/

            plot->addPoint(Point{time_s,speed_kmh}, QString::fromStdString(car->getName()));

        }

        plot->update();



        //
     //


    }
}

}
