#include "roadbuilder.h"
#include "ui/uimanager.h"
#include "ui/map/mapui.h"
#include "ui/map/pathlayer.h"
#include "ui/map/nodeui.h"
#include <QGuiApplication>

#include "core/sim/simulation.h"
#include "ui/editor/selectionhandler.h"

#include "ui/qteventscheduler.h"
#include "ui/map/mapui.h"

namespace LIMOSIM
{

RoadBuilder *roadBuilderInstance = 0;

RoadBuilder::RoadBuilder(QObject *_parent) :
    QObject(_parent),
    p_map(Map::getInstance()),
    p_uiManager(UiManager::getInstance()),
    p_pathLayer(0),
    p_selectedNode(0),
    p_selectedSegment(0),
    p_currentWay(0)
{
    roadBuilderInstance = this;
}

RoadBuilder* RoadBuilder::getInstance()
{
    if(!roadBuilderInstance)
        roadBuilderInstance = new RoadBuilder();

    return roadBuilderInstance;
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void RoadBuilder::clear()
{
    p_selectedNode = 0;
    p_selectedSegment = 0;
    p_currentWay = 0;
}

void RoadBuilder::handleKey(int _key)
{
    QtEventScheduler *scheduler = dynamic_cast<QtEventScheduler*>(Simulation::getInstance()->getEventScheduler());
    if(_key==Qt::Key_Delete)
    {
        if(p_selectedNode)
            deleteNode(p_selectedNode);
        else if(p_selectedSegment)
            deleteSegment(p_selectedSegment);
    }
    else if(_key==Qt::Key_Right)
        scheduler->step();
    else if(_key==Qt::Key_Up)
        scheduler->start();
    else if(_key==Qt::Key_Down)
        scheduler->stop();
    else if(_key>=48 && _key<=57 ) //  numeric
    {
        int value = _key - 48;
        bool ctrlPressed = (QGuiApplication::keyboardModifiers()==Qt::ControlModifier);

        SelectionHandler *selectionHandler = SelectionHandler::getInstance();
    }
    else if(_key==Qt::Key_A)
    {
        Car *car = Map::getInstance()->getCar("0");
        car->switchToLeftLane();
    }
    else if(_key==Qt::Key_D)
    {
        Car *car = Map::getInstance()->getCar("0");
        car->switchToRightLane();
    }
    else if(_key==Qt::Key_Space)
        MapUi::getInstance()->centerInViewPort();
    else if(_key==Qt::Key_Plus)
    {
        MapUi::getInstance()->zoomIn();
    }
    else if(_key==Qt::Key_Minus)
    {
        MapUi::getInstance()->zoomOut();
    }
    else
        qDebug() << "RoadBuilder::handleKey" << _key;
}

void RoadBuilder::registerNode(NodeUi *_node)
{
    connect(_node, SIGNAL(leftClicked()), this, SLOT(onNodeLeftClicked()));
    connect(_node, SIGNAL(rightClicked()), this, SLOT(onNodeRightClicked()));
}

void RoadBuilder::selectNode(NodeUi *_node)
{
    // deselect the previous node
    if(p_selectedNode)
        p_selectedNode->setSelection(SELECTION::NO_SELECTION);
    if(p_selectedSegment)
    {
        p_selectedSegment->setSelection(SELECTION::NO_SELECTION);
        p_selectedSegment = 0;
    }

    // select the new node
    p_selectedNode = _node;
    p_selectedNode->setSelection(SELECTION::HARD_SELECTION);
}

void RoadBuilder::deleteNode(NodeUi *_node)
{
    // unhighlight all highlighted destinates
    QList<NodeUi*> destinations = p_selectedNode->updateDestinations();
    for(int i=0; i<destinations.size(); i++)
        destinations.at(i)->softDeselection();

    // delete the ui part
    p_uiManager->removeNode(_node);

    p_selectedNode = 0;
}

void RoadBuilder::registerSegment(SegmentMarker *_segment)
{
    connect(_segment, SIGNAL(leftClicked()), this, SLOT(onSegmentLeftClicked()));
}

void RoadBuilder::selectSegment(SegmentMarker *_segment)
{
    // deselect the previous node
    if(p_selectedSegment)
        p_selectedSegment->setSelection(SELECTION::NO_SELECTION);
    if(p_selectedNode)
    {
        p_selectedNode->setSelection(SELECTION::NO_SELECTION);
        p_selectedNode = 0;
    }

    // select the new node
    p_selectedSegment = _segment;
    p_selectedSegment->setSelection(SELECTION::HARD_SELECTION);
}

void RoadBuilder::deleteSegment(SegmentMarker *_segment)
{
    // TODO:
    Segment *segment = _segment->getSegment();
    segment->getWay()->removeSegment(segment);


    // delete the ui part
    p_uiManager->removeSegment(_segment);

    p_pathLayer->update();

    p_selectedSegment = 0;
}

void RoadBuilder::deselectAll()
{
    if(p_selectedNode)
    {
        p_selectedNode->setSelection(SELECTION::NO_SELECTION);
        p_selectedNode = 0;
    }
    if(p_selectedSegment)
    {
        p_selectedSegment->setSelection(SELECTION::NO_SELECTION);
        p_selectedSegment = 0;
    }
    p_currentWay = 0;
}

void RoadBuilder::registerPathLayer(PathLayer *_pathLayer)
{
    p_pathLayer = _pathLayer;

    connect(p_pathLayer, SIGNAL(mapPositionClicked(Position)), this, SLOT(onMapPositionClicked(Position)));

}

NodeUi* RoadBuilder::createNode(const Position &_position, bool _link)
{
    // create a new node
    Node *node = p_map->createNode(_position);
    node->setPosition(_position);
    NodeUi *ui = p_uiManager->addNode(node);

    // if there is no way yet, create a new one
    if(_link && p_selectedNode && !p_currentWay)
        createWayWithNode();

    // select the new node
    selectNode(ui);



    return ui;
}

void RoadBuilder::createSegmentUi(Segment *_segment)
{
    // create a new segment marker and update the path layer to display the way
    if(_segment)
    {
        _segment->linkLanes();
        p_uiManager->addSegment(_segment);


        p_uiManager->getNodeUi(_segment->getStartGate()->node)->handleMovement();
        p_uiManager->getNodeUi(_segment->getEndGate()->node)->handleMovement();


        p_pathLayer->update();
    }
}

void RoadBuilder::createWayWithNode()
{
    p_currentWay = p_map->createWay();
    p_currentWay->setType(WAY_TYPE::PRIMARY_HIGHWAY);
    //p_currentWay->setIsOneway(true);
    //p_currentWay->setLanes(1, 1, 0, 0);

    p_currentWay->addNode(p_selectedNode->getNode());

}


/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void RoadBuilder::onMapPositionClicked(const Position &_position)
{
    if(QGuiApplication::keyboardModifiers()==Qt::ShiftModifier)
    {
        NodeUi *node = createNode(_position, true);


        // link the node
        if(p_currentWay)
        {
            Segment *segment = p_currentWay->addNode(node->getNode());
            createSegmentUi(segment);
        }
    }
    else
    {
        deselectAll();
        SelectionHandler::getInstance()->clearSelection();
    }

}

void RoadBuilder::onNodeLeftClicked()
{
    NodeUi *nodeUi = qobject_cast<NodeUi*>(QObject::sender());
    if(nodeUi)
    {
        // select the node
        selectNode(nodeUi);

        p_currentWay = 0;
    }
}

void RoadBuilder::onNodeRightClicked()
{
    bool modifier = QGuiApplication::keyboardModifiers()==Qt::ShiftModifier;

    NodeUi *nodeUi = qobject_cast<NodeUi*>(QObject::sender());
    if(modifier && nodeUi && nodeUi!=p_selectedNode) // ensure that start and destination are different
    {
        if(!p_currentWay && p_selectedNode)
            createWayWithNode();

        //
        if(p_currentWay)
        {
            Segment *segment = p_currentWay->addNode(nodeUi->getNode());
            createSegmentUi(segment);

            // select the destination node
            selectNode(nodeUi);
        }

    }

}

void RoadBuilder::onSegmentLeftClicked()
{
    SegmentMarker *segment = qobject_cast<SegmentMarker*>(QObject::sender());
    if(segment)
    {
        selectSegment(segment);
    }
}

}
