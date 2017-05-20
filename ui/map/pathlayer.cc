#include "pathlayer.h"
#include "LIMoSim/map/segment.h"
#include "LIMoSim/map/way.h"
#include "LIMoSim/map/node.h"
#include "LIMoSim/map/lane.h"
#include "LIMoSim/location/math_.h"
#include "ui/uimanager.h"
#include "ui/editor/roadbuilder.h"
#include <iostream>
#include <QGuiApplication>
#include <algorithm>




namespace LIMoSim
{

PathLayer::PathLayer(QQuickItem *_parent) :
    MapElementUi(_parent),
    p_map(Map::getInstance()),
    p_highlightedWay(0),
    p_highlightedCar(0)
{
    setZ(10);

    setWidth(p_settings->mapWidth);
    setHeight(p_settings->mapHeight);


    setAcceptedMouseButtons(Qt::AllButtons);

    RoadBuilder::getInstance()->registerPathLayer(this);

    connect(p_eventManager, SIGNAL(carSelected(CarUi*)), this, SLOT(onCarSelection(CarUi*)));
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void PathLayer::setPath(const std::vector<Node*> &_path)
{
    m_path = _path;

    update();
}

void PathLayer::setHighlightedWay(Way *_way)
{
    p_highlightedWay = _way;

    update();
}

void PathLayer::clear()
{
    update();
}

/*************************************
 *              EPS EXPORT           *
 ************************************/

QString PathLayer::toEps()
{
    QString data;
    int zoomLevel = p_settings->getZoomLevel();

    data += exportIntersections(zoomLevel);

    std::map<std::string,Way*> ways = p_map->getWays();
    std::map<std::string,Way*>::iterator waysIt;
    for(waysIt=ways.begin(); waysIt!=ways.end(); waysIt++)
    {
        Way *way = waysIt->second;

        if(zoomLevel==ZOOM_LEVEL::PATH)
            data += exportWay(way);

        std::vector<Segment*> segments = way->getSegments();
        for(unsigned int i=0; i<segments.size(); i++)
        {
            Segment *segment = segments.at(i);
            data += exportSegment(segment, zoomLevel);

            if(zoomLevel==ZOOM_LEVEL::DETAIL)
                data += exportLanes(segment);
        }
    }

    // connection lanes
    if(zoomLevel==ZOOM_LEVEL::DETAIL)
    {
        std::vector<Node*> nodes = p_map->getNodesList();
        for(unsigned int i=0; i<nodes.size(); i++)
        {
            Node *node = nodes.at(i);
            data += exportConnectionLanes(node);
        }
    }


    // trip data
    QString tripData = "477807,52919181,677231548,677231549,271159786,677231503,677231498,677231367,677231366,130117508,130117546,677231297,677231298,677231291,677231290,677231274,677231273,677231270,677230898,677230881,52919183,677230875,677230875,4346024044,677231808,677231810,160374,620856102,3570202330,313473120,32675941,3783376390,3570202327,846451200,1473417250,282657371,88300981,361358711,243090862,160375,249459236,243090861,302799818,362654646,1124823705,246584324,249398913,461153104,32558269,1448929542,249398800,1496519940,268566714,160388,243092689,249457378,153541711,3751855856,273109944,153541731,1647851192,275672221,275672221,1491580853,3751855835,273109929,1491580857,3751855907,32558270,311584339,249539596,249539595,249396642,249539593,255159153,249539594,1432513821,4625438212,164522,3451322093,461153115,2881724656,268556148,2482077605,268558637,268563612,249538986,3569208993,3569208993,477805,1647851191,3441544701,286531881,286567531,254461745,2023126444,290386771,1120120820,2023126442,4690495585,268456660,3441540731,243091055,3441540650,269614115,2147803005,3441538569,269698493,269696747,267842727,269697937,267842725,125348776,164317,3441521491,477807";
    QStringList trip = tripData.split(",");

    QPolygonF polygon;
    for(int i=0; i<trip.size(); i++)
    {
        Node *node = p_map->getNode(trip.at(i).toStdString());
        polygon << getCanvasPoint(node->getPosition());
    }
    data += m_eps.drawPolygon(polygon, LineStyle("green", 3, true));



    // handover positions
    QList<Position> positions;
    positions << Position(661.875,  429.340);
    positions << Position(1121.210,   501.344);
    positions << Position(1103.490,   707.166);
    positions << Position(329.880,   784.974);




    for(int i=0; i<positions.size(); i++)
    {
        int s = 12;
        LineStyle style("red", 3);
        QPointF center = getCanvasPoint(positions.at(i));

        data += m_eps.drawCircle(center, s, LineStyle("darkgray", s, true, true));
        data += m_eps.drawCircle(center, s, LineStyle("black", 1.5, true, false));
        data += m_eps.drawText(QString::number(i + 1), center, TextStyle("Arial", 22));


        // cross marker
        //data += m_eps.drawLine(center-QPointF(s,s), center+QPointF(s,s), style);
        //data += m_eps.drawLine(center-QPointF(-s,s), center+QPointF(-s,s), style);


        // triangle marker
        /*
        QPolygonF triangle;
        triangle << center + QPointF(0, s);
        triangle << center + QPointF(s, -s);
        triangle << center + QPointF(-s, -s);

        data += m_eps.drawPolygon(triangle, LineStyle("red", 3, true));*/
    }



    return data;
}



QString PathLayer::exportIntersections(int _zoomLevel)
{
    QString data;
    std::map<std::string,Node*> &nodes = p_map->getNodes();
    std::map<std::string,Node*>::iterator it;
    for(it=nodes.begin(); it!=nodes.end(); it++)
    {
        Node *node = it->second;
        if(node->getConnectionLanes().size())
        {

            QList<Position> path = getIntersectionPath(node);
            QPolygonF polygon;
            for(int i=0; i<path.size(); i++)
                polygon << getCanvasPoint(path.at(i));
            QPainterPath painterPath;
            painterPath.addPolygon(polygon);


            data += m_eps.drawPath(painterPath, LineStyle("black", 1, true, true));

            //
            if(_zoomLevel==ZOOM_LEVEL::DETAIL)
            {
                for(int i=0; i<path.size(); i+=2)
                {
                    Position current = path.at(i);
                    Position previous = (i!=0) ? path.at(i-1) : path.at(path.size()-1);

                    data += m_eps.drawLine(getCanvasPoint(previous), getCanvasPoint(current), LineStyle("white"));
                }
            }

        }
    }

    return data;
}

QString PathLayer::exportWay(Way *_way)
{
    QPolygonF polygon;

    std::vector<Node*>& nodes = _way->getNodes();
    for(unsigned int i=0; i<nodes.size(); i++)
    {
        Node *currentNode = nodes.at(i);
        polygon << getCanvasPoint(currentNode->getPosition()).toPoint();
    }

    QPainterPath path;
    path.addPolygon(polygon);

    return m_eps.drawPath(path, LineStyle());
}

QString PathLayer::exportSegment(Segment *_segment, int _zoomLevel)
{
    QString data;

    Rect rect = _segment->getRect();
    QPolygonF polygon;
    polygon << getCanvasPoint(rect.startLeft);
    polygon << getCanvasPoint(rect.startRight);
    polygon << getCanvasPoint(rect.endRight);
    polygon << getCanvasPoint(rect.endLeft);

    QPainterPath path;
    path.addPolygon(polygon);
    LineStyle style = p_settings->segmentStyle;
    style.fill = true;
    style.close = true;
    data += m_eps.drawPath(path, style);

    //
    if(_zoomLevel==ZOOM_LEVEL::DETAIL)
    {
        data += m_eps.drawLine(getCanvasPoint(rect.startLeft), getCanvasPoint(rect.endLeft), LineStyle("white"));
        data += m_eps.drawLine(getCanvasPoint(rect.startRight), getCanvasPoint(rect.endRight), LineStyle("white"));
    }

    return data;
}

QString PathLayer::exportLanes(Segment *_segment)
{
    QString data;
    std::vector<Lane*> lanes = _segment->getLanes();

    Lane *previous = 0;
    for(unsigned int i=0; i<lanes.size(); i++)
    {
        Lane *lane = lanes.at(i);
        data += exportLane(lane, LineStyle("gray"));

        if(previous)
        {
            Node *startNode = lane->getStartEndpoint()->node;
            Node *endNode = lane->getEndEndpoint()->node;
            Position start = (lane->getStartEndpoint()->position + previous->getEndpointForNode(startNode)->position) / 2;
            Position end = (lane->getEndEndpoint()->position + previous->getEndpointForNode(endNode)->position) / 2;

            qDebug() << previous->getDirectionType() << lane->getDirectionType();

            if(previous->getDirectionType()==lane->getDirectionType())
                data += m_eps.drawLine(getCanvasPoint(start), getCanvasPoint(end), LineStyle("white", 1, false, false, Qt::DashLine));
            else
                data += m_eps.drawLine(getCanvasPoint(start), getCanvasPoint(end), LineStyle("white"));
        }

        previous = lane;
    }


    return data;
}

QString PathLayer::exportLane(Lane *_lane, const LineStyle &_style)
{
    QPointF from = getCanvasPoint(_lane->getStartEndpoint()->position);
    QPointF to = getCanvasPoint(_lane->getEndEndpoint()->position);
    return m_eps.drawLine(from, to, _style);
}

QString PathLayer::exportConnectionLanes(Node *_node)
{
    QString data;
    std::map<std::pair<Lane*,Lane*>,Lane*> connectionLanes = _node->getConnectionLanes();
    std::map<std::pair<Lane*,Lane*>,Lane*>::iterator lanes;
    for(lanes=connectionLanes.begin(); lanes!=connectionLanes.end(); lanes++)
        data += exportLane(lanes->second, LineStyle("blue"));

    return data;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void PathLayer::handlePaintEvent()
{
    int zoomLevel = p_settings->getZoomLevel();
    drawIntersections(zoomLevel);
    drawWays(zoomLevel);

    if(m_path.size()>1)
    {
        Node *node = m_path.at(0);
        for(unsigned int i=1; i<m_path.size(); i++)
        {
            Node *currentNode = m_path.at(i);
            if(node->isReachableNeighbor(currentNode))
                drawLine(node->getPosition(), currentNode->getPosition(), LineStyle("yellow"));
            else
                drawLine(node->getPosition(), currentNode->getPosition(), LineStyle("red"));

            node = currentNode;
        }
    }

    // TODO: call periodically without repainting the whole map


    QList<CarUi*> cars = SelectionHandler::getInstance()->getSelectedCars();
    for(int c=0; c<cars.size(); c++)
    {
        CarUi *ui = cars.at(c);
        Car *car = ui->getCar();

        std::vector<Position> positions;
        positions.push_back(car->getPosition());
        positions.push_back(car->getPositionInfo().currentNode->getPosition());


        PositionInfo info = car->getPositionInfo();
        for(unsigned int i=0; i<info.path.size(); i++)
        {
            positions.push_back(info.path.at(i)->getPosition());
        }

        //
        if(positions.size()>1)
        {
            Position lastPosition = positions.at(0);
            for(unsigned int i=0; i<positions.size(); i++)
            {
                Position position = positions.at(i);
                drawLine(lastPosition, position, LineStyle("yellow"));

                lastPosition = position;
            }
        }
    }



    if(p_highlightedCar)
    {
        p_painter->setBrush(QBrush("yellow"));
        p_painter->setPen("yellow");
        double r = 15;

        QPointF p = p_highlightedCar->position();
        p.setX(p.x() + p_highlightedCar->width()/2);
        p.setY(p.y() + p_highlightedCar->height()/2);

        p_painter->drawEllipse(p, r, r);
    }



}

void PathLayer::drawIntersections(int _zoomLevel)
{
    std::map<std::string,Node*> &nodes = p_map->getNodes();
    std::map<std::string,Node*>::iterator it;
    for(it=nodes.begin(); it!=nodes.end(); it++)
    {
        Node *node = it->second;
        if(node->getConnectionLanes().size())
        {
            QList<Position> path = getIntersectionPath(node);
            QPolygonF polygon;
            for(int i=0; i<path.size(); i++)
                polygon << getUiPosition(path.at(i));
            QPainterPath painterPath;
            painterPath.addPolygon(polygon);

            p_painter->fillPath(painterPath, QBrush("black"));


            if(_zoomLevel==ZOOM_LEVEL::DETAIL)
            {
                for(int i=0; i<path.size(); i+=2)
                {
                    Position current = path.at(i);
                    Position previous = (i!=0) ? path.at(i-1) : path.at(path.size()-1);

                    drawLine(previous, current, LineStyle("white"));
                }
            }
        }
    }
}



void PathLayer::drawWays(int _zoomLevel)
{
    std::map<std::string,Way*> ways = p_map->getWays();
    std::map<std::string,Way*>::iterator waysIt;
    for(waysIt=ways.begin(); waysIt!=ways.end(); waysIt++)
    {
        Way *way = waysIt->second;


        //
        std::vector<Node*>& nodes = way->getNodes();
        if(nodes.size()>1)
        {
            Node *node = nodes.at(0);
            for(unsigned int i=1; i<nodes.size(); i++)
            {
                Node *currentNode = nodes.at(i);

                Vector3d start = node->getPosition();
                Vector3d end = currentNode->getPosition();

                if(way==p_highlightedWay)
                    drawLine(start, end, LineStyle("red"));
                else
                    drawLine(start, end, LineStyle("black"));

                //if(!way->isBidirectional())
                //    drawDirection(start, end);

                node = currentNode;
            }
        }


        // segments
        std::vector<Segment*> segments = way->getSegments();
        for(unsigned int i=0; i<segments.size(); i++)
            drawSegment(segments.at(i), _zoomLevel);
    }


    // conection lanes
    if(_zoomLevel==ZOOM_LEVEL::DETAIL)
    {
        std::vector<Node*> nodes = p_map->getNodesList();
        for(unsigned int i=0; i<nodes.size(); i++)
        {
            Node *node = nodes.at(i);

            std::map<std::pair<Lane*,Lane*>,Lane*> connectionLanes = node->getConnectionLanes();
            std::map<std::pair<Lane*,Lane*>,Lane*>::iterator lanes;
            for(lanes=connectionLanes.begin(); lanes!=connectionLanes.end(); lanes++)
            {
                Lane *lane = lanes->second;

                Position start = lane->getStartEndpoint()->position;
                Position end = lane->getEndEndpoint()->position;

                drawLine(start, end, LineStyle("blue"));
            }
        }
    }



}

void PathLayer::drawSegment(Segment *_segment, int _zoomLevel)
{
    Rect rect = _segment->getRect();

    //if (!(p_mapUi->isVisible(rect.startLeft) || p_mapUi->isVisible(rect.startRight) || p_mapUi->isVisible(rect.endRight) || p_mapUi->isVisible(rect.endLeft)))
    //    return;

    QPolygonF polygon;
    polygon << getUiPosition(rect.startLeft);
    polygon << getUiPosition(rect.startRight);
    polygon << getUiPosition(rect.endRight);
    polygon << getUiPosition(rect.endLeft);

    QPainterPath path;
    path.addPolygon(polygon);



    p_painter->fillPath(path, QBrush(p_settings->segmentStyle.color.toStdString().c_str()));

    if(_zoomLevel==ZOOM_LEVEL::DETAIL)
    {
        drawLine(rect.startLeft, rect.endLeft, LineStyle("white"));
        drawLine(rect.startRight, rect.endRight, LineStyle("white"));


        std::vector<Lane*> lanes = _segment->getLanes();
        Lane *previous = 0;
        for(unsigned int l=0; l<lanes.size(); l++)
        {
            Lane *lane = lanes.at(l);
            QString color = p_settings->wayColors.value(lane->getDirectionType());
            if(_segment->getWay()==p_highlightedWay)
                color = "red";

      //      drawLine(lane->getStartEndpoint()->position, lane->getEndEndpoint()->position, color);

            if(previous)
            {
                Node *startNode = lane->getStartEndpoint()->node;
                Node *endNode = lane->getEndEndpoint()->node;
                Position start = (lane->getStartEndpoint()->position + previous->getEndpointForNode(startNode)->position) / 2;
                Position end = (lane->getEndEndpoint()->position + previous->getEndpointForNode(endNode)->position) / 2;

                if(previous->getDirectionType()==lane->getDirectionType())
                    drawLine(start, end, LineStyle("white", 1, false, false, Qt::DashLine));
                else
                    drawLine(start, end, LineStyle("white"));
            }

            previous = lane;
        }
    }
}

void PathLayer::drawDirection(const Vector3d &_from, const Vector3d &_to)
{
    Vector3d dir = (_to-_from).normed();
    Vector3d lDir(-dir.y, dir.x);
    Vector3d rDir(dir.y, -dir.x);
    Vector3d a1 = (dir + lDir).normed();
    Vector3d a2 = (dir + rDir).normed();

    double w = 1.5;

    double length = (_to-_from).norm();
    Vector3d c = _from + dir * length * 1/4;
    drawLine(c - a1*w, c);
    drawLine(c, c - a2 * w);

    c = _from + dir * length * 3/4;
    drawLine(c - a1*w, c);
    drawLine(c, c - a2 * w);
}

void PathLayer::mousePressEvent(QMouseEvent *_event)
{
    emit mapPositionClicked(getMapPosition(_event->pos()));

    m_lastPosition = _event->pos();

    QPointF p = p_mapUi->getVirtualCanvasPosition(_event->pos());
    qDebug() << "clicked:" << _event->pos() << QString::fromStdString(getMapPosition(_event->pos()).toString()) << p;

    if(_event->button()==Qt::RightButton)
    {
        p_mapUi->centerAt(p);
    }
    else if(_event->button()==Qt::MiddleButton)
    {
        p_mapUi->handleScale(p_settings->getScaleFactor()+0.1, 0.1, _event->pos().x(), _event->pos().y());
    }

}

void PathLayer::mouseMoveEvent(QMouseEvent *_event)
{
    QPointF position = _event->pos();
    QPointF offset = position - m_lastPosition;
    //offset *= 0.75;

    //p_mapUi->setPosition(p_mapUi->position() + offset);
    p_mapUi->setOffset(p_mapUi->getOffset() + offset);

    m_lastPosition = position;
}

QList<Position> PathLayer::getIntersectionPath(Node *_node)
{
    std::vector<std::pair<double,Segment*>> sortedSegments;
    std::vector<Segment*> &segments = _node->getSegments();
    for(unsigned int i=0; i<segments.size(); i++)
    {
        Segment *segment = segments.at(i);
        Node *destination = segment->getOtherNode(_node);

        double angle = Math::computeRotation(_node->getPosition(), destination->getPosition());

        sortedSegments.push_back(std::pair<double,Segment*>(angle, segment));
    }
    std::sort(sortedSegments.begin(), sortedSegments.end());



    QList<Position> path;
    for(unsigned int i=0; i<sortedSegments.size(); i++)
    {
        Segment *segment = sortedSegments.at(i).second;

        Rect rect = segment->getRect();
        if(_node==segment->getStartGate()->node)
        {
            path << rect.startRight;
            path << rect.startLeft;
        }
        else
        {
            path << rect.endLeft;
            path << rect.endRight;
        }
    }


    return path;
}


/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void PathLayer::onCarSelection(CarUi *_car)
{
    p_highlightedCar = _car;

      // TODO: on map click, deselect the car
}


}
