#include "uieventmanager.h"
#include "ui/map/nodeui.h"
#include "uimanager.h"
#include "LIMoSim/map/node.h"
#include "LIMoSim/map/map.h"
#include "LIMoSim/mobility/routing/dijkstra.h"

#include "ui/map/mapui.h"
#include "ui/map/pathlayer.h"
#include "ui/map/segmentmarker.h"

#include <QGuiApplication>

namespace LIMoSim
{

UiEventManager *uiEventManagerInstance = 0;

UiEventManager::UiEventManager(QObject *_parent) :
    QObject(_parent),
    p_start(0),
    p_destination(0)
{
    uiEventManagerInstance = this;
}

UiEventManager* UiEventManager::getInstance()
{
    if(!uiEventManagerInstance)
        uiEventManagerInstance = new UiEventManager();

    return uiEventManagerInstance;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void UiEventManager::route(NodeUi *_from, NodeUi *_to)
{
    Map *map = Map::getInstance();
    Node *avoid = 0;


    RoutingGraph graph = map->createGraph(_from->getNode(), _to->getNode(), avoid);
    Node *start = _from->getNode();
    Node *destination = _to->getNode();

    graph.info();

    // perform the routing
    Dijkstra dijkstra;
    dijkstra.setGraph(&graph);
    RoutingPath routingPath = dijkstra.computeShortestPath(graph.getNode(start), graph.getNode(destination));



    emit display(QString::fromStdString(routingPath.toString()));



   // avoid = 0;
    std::vector<Node*> path = map->convertRoutingPath(routingPath, avoid);
    if(path.size()>1)
    {
        PathLayer *pathLayer = MapUi::getInstance()->getPathLayer();
        pathLayer->setPath(path);
    }
    else
    {
        std::cout << "path invalid " << std::endl;
    }

    graph.clear();


}

void UiEventManager::handleSegmentSelection(SegmentMarker *_segment)
{
    emit segmentSelected(_segment);
}

void UiEventManager::handleCarSelection(CarUi *_car)
{
    emit carSelected(_car);
}

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void UiEventManager::onNodeMoved()
{
    NodeUi *ui = qobject_cast<NodeUi*>(QObject::sender());
    if(ui)
    {
        emit nodeMoved(ui->getNode());

        if(p_start && p_destination)
            route(p_start, p_destination);
    }
}

void UiEventManager::onNodeLeftClicked()
{
    if((QGuiApplication::keyboardModifiers()==Qt::ControlModifier))
        p_start = qobject_cast<NodeUi*>(QObject::sender());

}

void UiEventManager::onNodeRightClicked()
{
    if((QGuiApplication::keyboardModifiers()==Qt::ControlModifier))
        p_destination = qobject_cast<NodeUi*>(QObject::sender());

    if(p_destination && p_start)
    {
        route(p_start, p_destination);
    }


}

void UiEventManager::onNodeHovered()
{
    NodeUi *node = qobject_cast<NodeUi*>(QObject::sender());
    QList<NodeUi*> destinations = node->updateDestinations();
    for(int i=0; i<destinations.size(); i++)
        destinations.at(i)->softSelection();
}

void UiEventManager::onNodeUnhovered()
{
    NodeUi *node = qobject_cast<NodeUi*>(QObject::sender());
    QList<NodeUi*> destinations = node->updateDestinations();
    for(int i=0; i<destinations.size(); i++)
        destinations.at(i)->softDeselection();
}

}
