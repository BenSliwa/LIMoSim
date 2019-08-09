#include "uimanager.h"
#include <QQmlContext>

#include "LIMoSim/map/map.h"
#include "LIMoSim/map/node.h"
#include "LIMoSim/map/way.h"

#include "ui/map/pathlayer.h"
#include "ui/map/carui.h"
#include "ui/map/distanceindicator.h"
#include "ui/uisettings.h"


#include "LIMoSim/settings/filehandler.h"
#include "LIMoSim/settings/domelement.h"

#include "LIMoSim/sim/rng.h"
#include "LIMoSim/map/nodegrid.h"

#include "LIMoSim/map/osm/osmdocument.h"
#include "LIMoSim/settings/xmlparser.h"

#include "LIMoSim/location/wgs84.h"

#include "LIMoSim/map/trafficsignal.h"


#include "ui/plot/figure.h"
#include "ui/plot/plot.h"

#include "ui/plot/uielement.h"

#include "ui/qteventscheduler.h"

namespace LIMoSim
{

UiManager *uiManagerInstance = nullptr;

UiManager::UiManager(QObject *_parent) :
    QObject(_parent),
    EventHandler(),
    p_map(nullptr)
{
    uiManagerInstance = this;

    p_editor = new RoadBuilder(this);


    loadQml();

    generateUiElements();

    //
    m_updateInterval_s = 25.0/1000.0;
    m_updateTimer = new Event(m_updateInterval_s, this, "UiManager::updateTimer");
    scheduleEvent(m_updateTimer);
}

UiManager::~UiManager()
{
    deleteEvent(m_updateTimer);
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

UiManager* UiManager::getInstance()
{
    if(!uiManagerInstance)
        uiManagerInstance = new UiManager();

    return uiManagerInstance;
}

void UiManager::loadQml()
{
    UiSettings *settings = UiSettings::getInstance();
    QQmlContext *context = m_qml.rootContext();

    // register singletons
    context->setContextProperty("UiManager", this);
    context->setContextProperty("UiEventManager", &m_uiEventManager);
    context->setContextProperty("Editor", p_editor);
    context->setContextProperty("Settings", settings);

    // register types
    qmlRegisterType<MapUi>("com.LIMOSIM", 1, 0, "MapUi");
    qmlRegisterType<ViewPort>("com.LIMOSIM", 1, 0, "ViewPort");
    qmlRegisterType<Figure>("com.LIMOSIM", 1, 0, "Figure");

    // register c++ classes
    qRegisterMetaType<SegmentMarker*>("SegmentMarker*");


    // load the actual qml file
    m_qml.load(QUrl(QLatin1String("qrc:/main.qml")));

    //    
    QObject *object = m_qml.rootObjects().first();
    p_map = object->findChild<MapUi*>("map");

    p_viewPort = ViewPort::getInstance();


    Map *map = Map::getInstance();
    MapBounds bounds = map->computeBounds();

    // set the scale factor for showing the whole map in the viewport
    double scaleFactor = std::min(p_viewPort->width() / bounds.width, p_viewPort->height() / bounds.height);
    settings->handleScale(scaleFactor, 0, 0);

    // set the offset the center the map in the viewport
    p_map->centerInViewPort();
    p_map->centerAtPosition(bounds.center);
}

bool UiManager::handleDrop(const QString &_path)
{
    QString path = _path;
    path.remove("file:///");

    return loadScenario(path);
}

bool UiManager::loadScenario(const QString &_path)
{
    QString path = _path;
    path = path.replace("file:///", "");

    qDebug() << "UiManager::loadScenario" << path;



    Map *map = Map::getInstance();


    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        XMLParser xml;
        WGS84 converter;
        OSMDocument document = OSMDocument::fromXML(xml.parse(path.toStdString()), converter);
    }
    else
        return false;


    std::map<std::string,Node*>& nodes = map->getNodes();
    std::map<std::string,Node*>::iterator it;
    for(it=nodes.begin(); it!=nodes.end(); it++)
        it->second->computeDestinations();



    generateUiElements();
    p_map->getPathLayer()->update();



    return true;
}

bool UiManager::saveScenario(const QString &_path)
{
    QString path = _path;
    path = path.replace("file:///", "");

    OSMDocument osm;
    DOMElement *document = osm.toXML();
    bool success = FileHandler::write(document->toString(), path.toStdString());
    delete document;

    return success;
}

void UiManager::generateUiElements()
{
    Map *map = Map::getInstance();

    // create the nodes
    std::map<std::string,Node*> nodes = map->getNodes();
    std::map<std::string,Node*>::iterator it;
    for(it=nodes.begin(); it!=nodes.end(); it++)
    {
        Node *node = it->second;
        if(!m_nodes.value(node))
            addNode(node);
    }

    // create the ways
    std::map<std::string,Way*> ways = map->getWays();
    std::map<std::string,Way*>::iterator w;
    for(w=ways.begin(); w!=ways.end(); w++)
    {
        Way *way = w->second;
        std::vector<Segment*> &segments = way->getSegments();
        for(auto segment : segments)
        {
            if(!m_segments.value(segment))
                addSegment(segment);
        }
    }


    // create the vehicles
    std::map<std::string,Car*> cars = map->getCars();
    std::map<std::string,Car*>::iterator c;
    for(c=cars.begin(); c!=cars.end(); c++)
    {
        Car *car = c->second;
        addCar(car);
    }




    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());
    scheduler->start();
}

void UiManager::clear()
{
    // delete all ways
    QMap<Segment*, SegmentMarker*>::iterator segment;
    for(segment=m_segments.begin(); segment!=m_segments.end(); segment++)
    {
        SegmentMarker *ui = segment.value();
        delete ui; // deleteLater is not working here
    }
    m_segments.clear();

    // delete all cars
    QMap<Car*, CarUi*>::iterator car;
    for(car=m_cars.begin(); car!=m_cars.end(); car++)
    {
        CarUi *ui = car.value();
        delete ui;
    }
    m_cars.clear();


    // delete all nodes
    QMap<Node*, NodeUi*>::iterator node;
    for(node=m_nodes.begin(); node!=m_nodes.end(); node++)
    {
        NodeUi *ui = node.value();
        ui->deleteLater();
    }
    m_nodes.clear();

    // delete the actual objects
    Map::getInstance()->clear();



    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());
    scheduler->reset();




    // clear and update the path layer
    p_map->getPathLayer()->clear();

    // reset the road builder
    RoadBuilder::getInstance()->clear();
}

void UiManager::exportScreenshot()
{
    qDebug() << "UiManager::exportScreenshot";

    QString data = UiElement::readFile(":/EpsHeader.txt").join("\n");
    data.replace("$BOX_WIDTH$", QString::number(p_map->getMapWidth()));
    data.replace("$BOX_HEIGHT$", QString::number(p_map->getMapHeight()));

    //data.replace("$BOX_WIDTH$", QString::number(20000));
    //data.replace("$BOX_HEIGHT$", QString::number(10000));


    // background
    data += p_map->toEps();


    // paths
    data += p_map->getPathLayer()->toEps();

    // nodes
    UiSettings *settings = UiSettings::getInstance();

    //if(settings->getValue("MapExport", "Nodes").toInt()==1)
    {
        QMap<Node*, NodeUi*>::iterator it;
        for(it=m_nodes.begin(); it!=m_nodes.end(); it++)
            data += it.value()->toEps();
    }

    // segments
    if(settings->getValue("MapExport", "Segments").toInt()==1)
    {
        QMap<Segment*, SegmentMarker*>::iterator it;
        for(it=m_segments.begin(); it!=m_segments.end(); it++)
            data += it.value()->toEps();
    }


    // cars
    if(settings->getValue("MapExport", "Cars").toInt()==1)
    {
        QMap<Car*, CarUi*>::iterator it;
        for(it=m_cars.begin(); it!=m_cars.end(); it++)
            data += it.value()->toEps();
    }

    data += p_viewPort->getDistanceIndicator()->toEps();

    data += "\n" + UiElement::readFile(":/EpsFooter.txt").join("\n");

    UiElement::writeToFile(data, "screen.eps");

}

void UiManager::start()
{
    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());
    scheduler->start();
}

void UiManager::stop()
{
    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());
    scheduler->stop();
}

void UiManager::step()
{
    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());
    scheduler->step();
}

void UiManager::setTimeScaleFactor(double _factor)
{
    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());
    scheduler->setTimeScaleFactor(_factor);
}

NodeUi* UiManager::getNodeUi(Node* _node)
{
    return m_nodes.value(_node);
}

NodeUi* UiManager::addNode(Node* _node)
{
    NodeUi *ui = new NodeUi(p_map);
    ui->setNode(_node);


    m_uiEventManager.connect(ui, SIGNAL(moved()), &m_uiEventManager, SLOT(onNodeMoved()));
    m_uiEventManager.connect(ui, SIGNAL(leftClicked()), &m_uiEventManager, SLOT(onNodeLeftClicked()));
    m_uiEventManager.connect(ui, SIGNAL(rightClicked()), &m_uiEventManager, SLOT(onNodeRightClicked()));
    m_uiEventManager.connect(ui, SIGNAL(hovered()), &m_uiEventManager, SLOT(onNodeHovered()));
    m_uiEventManager.connect(ui, SIGNAL(unhovered()), &m_uiEventManager, SLOT(onNodeUnhovered()));


    m_nodes[_node] = ui;

    return ui;
}

void UiManager::removeNode(NodeUi* _node)
{
    Node *node = _node->getNode();

    // destroy the object
    Map::getInstance()->removeNode(node);

    // delete the ui part

    _node->deleteLater();
    m_nodes.remove(node);

    // update the ui
    p_map->getPathLayer()->update();


    //
    emit nodeRemoved(node);

}

void UiManager::insertNode(Segment *_segment)
{

    Way *way = _segment->getWay();
    Segment *segment = way->insertNode(_segment);

    Segment *predecessor = segment->getPredecessor();
    Segment *successor = segment->getSuccessor();

    addNode(segment->getEndGate()->node);
    addSegment(segment);



    // update the originator segment
    m_segments.value(_segment)->setSegment(_segment);



    if(predecessor)
    {
        m_segments.value(predecessor)->setSegment(predecessor);
    }

    if(successor)
    {
        m_segments.value(successor)->setSegment(successor);
    }


    // update the ui
    p_map->getPathLayer()->update();
}

SegmentMarker* UiManager::addSegment(Segment *_segment)
{
    SegmentMarker *marker = new SegmentMarker(p_map);
    marker->setSegment(_segment);

    m_segments.insert(_segment, marker);

    return marker;
}

void UiManager::removeSegment(SegmentMarker *_segment)
{
    Segment *segment = _segment->getSegment();
    m_segments.remove(segment);
    _segment->deleteLater();

    emit segmentRemoved(segment);
}

CarUi* UiManager::addCar(Car *_car)
{
    CarUi *car = new CarUi(p_map);
    //car->setColor("black");
    car->setCar(_car);

    m_cars.insert(_car, car);

    return car;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

MapUi* UiManager::getMap()
{
    return p_map;
}

QMap<Node*, NodeUi*>& UiManager::getNodes()
{
    return m_nodes;
}

QMap<Segment*, SegmentMarker*>& UiManager::getSegments()
{
    return m_segments;
}

QMap<Car*, CarUi*>& UiManager::getCars()
{
    return m_cars;
}

void UiManager::handleEvent(Event *_event)
{
    if(_event==m_updateTimer)
    {
        // update all cars
        QMap<Car*,CarUi*>::iterator car;
        for(car=m_cars.begin(); car!=m_cars.end(); car++)
        {
            CarUi *ui = car.value();
            ui->update();
        }

        // update all traffic lights
        QMap<Node*,NodeUi*>::iterator node;
        for(node=m_nodes.begin(); node!=m_nodes.end(); node++)
        {
            TrafficSignal *light = node.key()->getTrafficSignal();
            if(light)
                node.value()->update();
        }



        // update the simulation statistics

        QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());
        emit updated(scheduler->getEventCount(), scheduler->getScheduledEventCount(), scheduler->getSimTime(), scheduler->getSimDuration());


        // reschedule the timer
        scheduleEvent(m_updateTimer, m_updateInterval_s);
    }
}

/*************************************
 *         CONVENIENCE METHODS       *
 ************************************/


/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/


/*************************************
 *           PRIVATE METHODS         *
 ************************************/

}
