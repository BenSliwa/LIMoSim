#include "nodeui.h"
#include "pathlayer.h"
#include "ui/uimanager.h"
#include "LIMoSim/map/trafficsignal.h"

namespace LIMoSim
{

NodeUi::NodeUi(QQuickItem *_parent) :
    MapElementUi(_parent),
    p_node(nullptr)
{
    setZ(20);

    p_editor->registerNode(this);

    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);


    connect(this, SIGNAL(moved()), p_eventManager, SLOT(onNodeMoved()));
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

QList<NodeUi*> NodeUi::updateDestinations()
{
    UiManager *uiManager = UiManager::getInstance();
    QList<NodeUi*> destinationNodes;
    std::vector<DestinationEntry> destinations = p_node->getOutgoingConnections();
    for(auto & destination : destinations)
        destinationNodes << uiManager->getNodeUi(destination.destination);

    return destinationNodes;
}

bool NodeUi::isVisible()
{
    bool visible = p_mapUi->isVisible(p_node->getPosition());

    return visible;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void NodeUi::setNode(Node *_node)
{
    p_node = _node;

    setHeight(15);
    setWidth(15);

    if(p_node->getType()==NODE_TYPE::TRAFFIC_SIGNAL)
    {
        setHeight(25);
        setWidth(10);
    }


    centerAt(getUiPosition(p_node->getPosition()));
}

Node* NodeUi::getNode()
{
    return p_node;
}

void NodeUi::handleMovement()
{
    p_node->setPosition(p_node->getPosition());

    emit moved();
}

/*************************************
 *              EPS EXPORT           *
 ************************************/

QString NodeUi::toEps()
{
    QString data;
    QPointF center = getCanvasPoint(p_node->getPosition());

    TrafficSignal *signal = p_node->getTrafficSignal();
    if(!signal) // regular node
    {
        //QRectF rect = getCenteredRect(center, 15, 15);
        //data += m_eps.drawRect(rect, LineStyle("green", 1, true, true));
    }
    else //
    {
        QRectF rect = getCenteredRect(center, 10, 25);
        data += m_eps.drawRect(rect, LineStyle("black", 1, true, true));

        int state = signal->getState();
        switch(state)
        {
            case TRAFFIC_SIGNAL::GREEN:
                data += exportLights("darkgray", "darkgray", "green");
            break;

            case TRAFFIC_SIGNAL::YELLOW:
                data += exportLights("darkgray", "yellow", "darkgray");
            break;

            case TRAFFIC_SIGNAL::RED:
                data += exportLights("red", "darkgray", "darkgray");
            break;

            case TRAFFIC_SIGNAL::RED_YELLOW:
                data += exportLights("red", "yellow", "darkgray");
            break;
        }
    }

    return data;
}

QString NodeUi::exportLights(const QString &_top, const QString &_center, const QString &_bottom)
{
    QString data;

    data += exportLight(2, _top);
    data += exportLight(1, _center);
    data += exportLight(0, _bottom);

    return data;
}

QString NodeUi::exportLight(int _index, const QString &_color)
{
    QString data;


    double h = height();
    double y = h / 3;

    LineStyle style(_color, 1, true, true);
    double r = 3;

    QPointF center = getCanvasPoint(p_node->getPosition());
    QPointF pos(0, y * (_index + 1) - h/2 - y/2);
    data += m_eps.drawCircle(center + pos, r, style);

    return data;
}


/*************************************
 *          ABSTRACT METHODS         *
 ************************************/


void NodeUi::handlePaintEvent()
{
    if(isVisible())
    {
        centerAt(getUiPosition(p_node->getPosition()));

        TrafficSignal *signal = p_node->getTrafficSignal();
        if(signal)
        {
            p_painter->fillRect(this->boundingRect(), "black");

            int state = signal->getState();
            switch(state)
            {
                case TRAFFIC_SIGNAL::GREEN:
                    drawLights("darkgray", "darkgray", "green");
                break;

                case TRAFFIC_SIGNAL::YELLOW:
                    drawLights("darkgray", "yellow", "darkgray");
                break;

                case TRAFFIC_SIGNAL::RED:
                    drawLights("red", "darkgray", "darkgray");
                break;

                case TRAFFIC_SIGNAL::RED_YELLOW:
                    drawLights("red", "yellow", "darkgray");
                break;

            }

        }
        else if(p_settings->drawNodes) //if(p_settings->getValue("MapExport", "Nodes").toInt()==1 )
        {
            QColor color = p_settings->nodeColors.value(m_selection);
            if(p_node->isIntersectionNode() && m_selection==SELECTION::NO_SELECTION)
                color = "red";

            p_painter->fillRect(this->boundingRect(), color);
        }
    }
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void NodeUi::drawLights(const QColor &_top, const QColor &_center, const QColor &_bottom)
{
    drawLight(0, _top);
    drawLight(1, _center);
    drawLight(2, _bottom);
}

void NodeUi::drawLight(int _index, const QColor &_color)
{
    double w = width();
    double h = height();
    double y = h / 3;

    p_painter->setBrush(QBrush(_color));
    p_painter->setPen(_color);
    double r = 3;

    p_painter->drawEllipse(QPointF(w/2, y * (_index + 1) - y/2), r, r);
}


void NodeUi::mousePressEvent(QMouseEvent *_event)
{
    m_dragActive = true;

    if(_event->button()==Qt::LeftButton)
        emit leftClicked();
    else if(_event->button()==Qt::RightButton)
        emit rightClicked();

    p_node->info();
}

void NodeUi::mouseReleaseEvent(QMouseEvent *_event)
{
    m_dragActive = false;

    //qDebug() << "NodeUi::mouseReleaseEvent" << QString::fromStdString(p_node->getName()) << QString::fromStdString(p_node->getPosition().toString());
}


void NodeUi::mouseMoveEvent(QMouseEvent *_event)
{
    // move the node

    // update the ui of all ways which contain the node -> UiManager map<Way,WayUi>


    if(m_dragActive)
    {

        QPointF point = mapToItem(p_mapUi,_event->pos());
        centerAt(point);
        p_mapUi->getPathLayer()->update();


        // TODO: only update the ways next to the node



        Position pos = getMapPosition(point);
        p_node->setPosition(pos);

        emit moved();

    }
}

void NodeUi::hoverEnterEvent(QHoverEvent *_event)
{
    if(m_selection==SELECTION::NO_SELECTION)
        setSelection(SELECTION::SOFT_SELECTION);

    p_node->info();

    emit hovered();
}

void NodeUi::hoverLeaveEvent(QHoverEvent *_event)
{
    if(m_selection==SELECTION::SOFT_SELECTION)
        setSelection(SELECTION::NO_SELECTION);

    emit unhovered();
}


}
