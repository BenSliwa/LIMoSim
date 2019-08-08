#include "segmentmarker.h"
#include "uimanager.h"
#include "pathlayer.h"
#include "LIMoSim/map/way.h"

namespace LIMoSim
{

SegmentMarker::SegmentMarker(QQuickItem *_parent) :
    MapElementUi(_parent),
    p_segment(nullptr),
    p_start(nullptr),
    p_end(nullptr),
    p_predecessor(nullptr),
    p_successor(nullptr)
{
    setZ(20);
    setHeight(10);
    setWidth(10);

    p_editor->registerSegment(this);

    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);

    UiManager *ui = UiManager::getInstance();
    connect(p_eventManager, SIGNAL(nodeMoved(Node*)), this, SLOT(onNodeMoved(Node*)));
    connect(ui, SIGNAL(nodeRemoved(Node*)), this, SLOT(onNodeRemoved(Node*)));
    connect(ui, SIGNAL(segmentRemoved(Segment*)), this, SLOT(onSegmentRemoved(Segment*)));
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

bool SegmentMarker::isVisible()
{
    bool visible = p_mapUi->isVisible(p_segment->getCenter());
    visible = visible && p_settings->drawSegmentMarkers;

    return visible;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void SegmentMarker::setSegment(Segment *_segment)
{
    p_segment = _segment;
    p_start = p_segment->getStartGate()->node;
    p_end = p_segment->getEndGate()->node;

    p_predecessor = p_segment->getPredecessor();
    p_successor = p_segment->getSuccessor();



    text = QString::fromStdString(p_segment->toString());

    centerAt(getUiPosition(p_segment->getCenter()));

}

Segment* SegmentMarker::getSegment()
{
    return p_segment;
}


void SegmentMarker::setName(const QString &_name)
{
    p_segment->getWay()->setName(_name.toStdString());
}

void SegmentMarker::setLanes(int _lanes)
{
    p_segment->getWay()->setLanes(_lanes);

    p_mapUi->getPathLayer()->update();
}

QString SegmentMarker::getName()
{
    return QString::fromStdString(p_segment->getWay()->getName());
}

int SegmentMarker::getLanes()
{
    return p_segment->getWay()->getLanes();
}


/*************************************
 *              EPS EXPORT           *
 ************************************/

QString SegmentMarker::toEps()
{
    QString data;

    QPointF center = getCanvasPoint(p_segment->getCenter());
    QRectF rect = getCenteredRect(center, 10, 10);

    data += m_eps.drawRect(rect, LineStyle("blue", 1, true, true));


    return data;
}

/*************************************
 *          ABSTRACT METHODS         *
 ************************************/

void SegmentMarker::handlePaintEvent()
{
    if(isVisible())
    {
        centerAt(getUiPosition(p_segment->getCenter()));

        QColor color = p_settings->segmentColors.value(m_selection);
        p_painter->fillRect(boundingRect(), color);
    }
}

void SegmentMarker::mousePressEvent(QMouseEvent *_event)
{
    //qDebug() << QString::fromStdString(p_segment->toString()) << p_predecessor << p_successor;

    if(_event->button()==Qt::LeftButton)
    {
        // select the way
        p_eventManager->handleSegmentSelection(this);

        emit leftClicked();
    }
    else if(_event->button()==Qt::RightButton)
    {
        // create a new node at the marker position
        p_uiManager->insertNode(p_segment);
    }
}

void SegmentMarker::mouseReleaseEvent(QMouseEvent *_event)
{

}

void SegmentMarker::hoverEnterEvent(QHoverEvent *_event)
{
    std::cout << p_segment->toString();
    if(p_predecessor)
        std::cout << "\tpre: " << p_predecessor->getStartGate()->node->getName() << " -> " << p_predecessor->getEndGate()->node->getName();
    if(p_successor)
        std::cout << "\tpost: "<< p_successor->getStartGate()->node->getName() << " -> " << p_successor->getEndGate()->node->getName();
    std::cout << std::endl;


    p_mapUi->getPathLayer()->setHighlightedWay(p_segment->getWay());
}

void SegmentMarker::hoverLeaveEvent(QHoverEvent *_event)
{
    p_mapUi->getPathLayer()->setHighlightedWay(nullptr);
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void SegmentMarker::onNodeRemoved(Node *_node)
{
    if(_node==p_start || _node==p_end)
    {
        if(_node==p_start && !p_predecessor)
            p_uiManager->removeSegment(this);
        else if(_node==p_end && !p_successor)
            p_uiManager->removeSegment(this);
        else
        {
            if(_node==p_end)
                p_uiManager->removeSegment(this);
            else
                setSegment(p_segment);
        }
    }
}

void SegmentMarker::onSegmentRemoved(Segment *_segment)
{
    if(_segment==p_predecessor || _segment==p_successor)
        setSegment(p_segment);
}

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void SegmentMarker::onNodeMoved(Node *_node)
{
    if(_node==p_start|| _node==p_end)
    {
        centerAt(getUiPosition(p_segment->getCenter()));

        // update the lanes
        p_segment->linkLanes();


    }

}



}
