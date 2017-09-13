#include "LIMoSim/location/math_.h"
#include "segment.h"
#include "lane.h"
#include "node.h"
#include "way.h"
#include <iostream>
#include <algorithm>
#include <cmath>

namespace LIMoSim
{

Segment::Segment(Node *_start, Node *_end, Way *_way) :
    p_way(_way),
    p_predecessor(0),
    p_successor(0)
{
    setStart(_start);
    setEnd(_end);
}

Segment::~Segment()
{
    // delete all lanes
    clear();

    // remove the segment entry from the nodes
    m_startGate.node->removeSegment(this);
    m_endGate.node->removeSegment(this);

    if(p_predecessor)
    {
        if(p_predecessor->getSuccessor()==this)
            p_predecessor->setSuccessor(0);
    }

    if(p_successor)
    {
        if(p_successor->getPredecessor()==this)
            p_successor->setPredecessor(0);
    }
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Segment::clear()
{
    for(unsigned int i=0; i<m_lanes.size(); i++)
    {
        Lane *lane = m_lanes.at(i);
        delete lane;
    }
}

void Segment::linkLanes()
{

    int lanes = p_way->getLanes();
    for(unsigned int i=0; i<lanes; i++)
    {
        updateLanePositions(i);
    }




    // TODO: only perform when necessary
    m_startGate.node->linkSegments();
    m_endGate.node->linkSegments();



}

void Segment::linkLanes(Lane *_out, LaneGate *_outGate, Lane *_in, LaneGate *_inGate, int _turnType)
{
    _outGate->out[_turnType] = _in;

    _inGate->addIncomingLane(_out, _turnType);
}

void Segment::clearLaneLinks(Node *_node)
{
    for(unsigned int i=0; i<m_lanes.size(); i++)
    {
        Lane *lane = m_lanes.at(i);
        LaneGate *gate = lane->getGateForNode(_node);
        if(gate)
            gate->clearOut();
    }
}

void Segment::connectTo(Segment *_segment, Node *_node, int _turnType, bool _direct)
{
    bool out = true;
    if(_node==m_startGate.node)
        out = false;

    bool in = true;
    if(_segment->getEndGate()->node==_node)
        in = false;

    //
    std::vector<Lane*> outLanes;
    out ? outLanes = this->getFowardLanes() : outLanes = this->getBackwardLanes();
    std::vector<Lane*> inLanes;
    in ? inLanes = _segment->getFowardLanes() : inLanes = _segment->getBackwardLanes();



    // connect the actual lanes
    int s = std::min(outLanes.size(), inLanes.size());
    for(unsigned int i=0; i<s; i++)
    {
        Lane *out = outLanes.at(i);
        Lane *in = inLanes.at(i);

        if(_direct)
        {
            linkLanes(out, out->getGateForNode(_node), in, in->getGateForNode(_node), _turnType);
        }
        else
        {
            updateConnectionLane(out, in, _node, _turnType);
        }
    }



}

void Segment::updateConnectionLane(Lane *_from, Lane *_to, Node *_node, int _turnType)
{
    Lane *connectionLane = _node->getConnectionLane(_from, _to);
    connectionLane->setSegment(this);
    connectionLane->setTurnType(_turnType);


    LaneEndpoint *outEndpoint = _from->getEndpointForNode(_node);
    LaneEndpoint *inEndpoint = _to->getEndpointForNode(_node);


    LaneEndpoint *connectionIn = connectionLane->getStartEndpoint();
    connectionIn->position = outEndpoint->position;

    LaneEndpoint *connectionOut = connectionLane->getEndEndpoint();
    connectionOut->position = inEndpoint->position;
    connectionOut->destination = _to->getOtherEndpoint(inEndpoint)->node;
    connectionOut->origin = _from->getOtherEndpoint(outEndpoint)->node;

    //std::cout << "CL: " << connectionIn->position.toString() << "\t" << connectionOut->position.toString() << "\t" << connectionOut->position.x << "\t" << connectionOut << std::endl;


    // link the lanes
    linkLanes(_from, &outEndpoint->gate, connectionLane, &connectionIn->gate, _turnType);
    linkLanes(connectionLane, &connectionOut->gate, _to, &inEndpoint->gate, TURN::THROUGH);
}

SegmentGate* Segment::getGateForNode(Node *_node)
{
    SegmentGate *gate = 0;
    if(_node==m_startGate.node)
        gate = getStartGate();
    else if(_node==m_endGate.node)
        gate = getEndGate();

    return gate;
}

void Segment::updateGatePosition(SegmentGate *_gate)
{
    Node *node = _gate->node;
    _gate->position = node->getPosition();
    double offset_m = 0;

    bool nodeCondition = false;
    if(node->getSegments().size()>2)
        nodeCondition = true;

    std::map<int,Segment*>::iterator it;
    for(it=_gate->segments.begin(); it!=_gate->segments.end(); it++)
    {
        int turnType = it->first;
        Segment *segment = it->second;
        Node *next = segment->getOtherNode(node);

        int type = segment->getWay()->getPathDirection(node, next);

        // reverse if the turn is left oriented
        if(turnType==TURN::SLIGHT_LEFT || turnType==TURN::LEFT || turnType==TURN::SHARP_LEFT)
            type==WAY_DIRECTION::FORWARD ? type=WAY_DIRECTION::BACKWARD : type=WAY_DIRECTION::FORWARD;

        double width_m = 0;
        if(type==WAY_DIRECTION::FORWARD)
            width_m = segment->getWay()->getForwardWidth();
        else
            width_m = segment->getWay()->getBackwardWidth();

        offset_m = std::max(offset_m, width_m);

        if(turnType==TURN::RIGHT || turnType==TURN::SHARP_RIGHT || turnType==TURN::LEFT || turnType==TURN::SHARP_LEFT)
            nodeCondition = true;
    }






    if(offset_m>0 && nodeCondition)
    {
        if(node==m_startGate.node)
            m_startGate.position = m_startGate.node->getPosition() + getDirection() * offset_m;
        else
            m_endGate.position = m_endGate.node->getPosition() + getDirection().revert() * offset_m;
    }





}

void Segment::updateLanePositions(int _index)
{
    //
    Position start = m_startGate.position;
    Position end = m_endGate.position;
    Vector3d dir = getDirection();
    Vector3d rightDir = dir.rotateRight();

    double width_m = 3.5;




    // determine the leftest points and move in right direction
    int left = p_way->getBackwardLanes();
    if(left)
    {
         start = start - (rightDir * width_m * left - rightDir * width_m/2);
         end = end - (rightDir * width_m * left - rightDir * width_m/2);
    }
    else
    {
        start = start + rightDir * width_m/2;
        end = end + rightDir * width_m/2;
    }

    Position laneStart = start + rightDir * width_m * _index;
    Position laneEnd = end + rightDir * width_m * _index;

    Vector3d startDir = (m_rect.startRight - m_rect.startLeft).normed();
    Vector3d endDir = (m_rect.endRight - m_rect.endLeft).normed();

    double startWidth = (m_rect.startRight - m_rect.startLeft).norm() / p_way->getLanes();
    double endWidth = (m_rect.endRight - m_rect.endLeft).norm() / p_way->getLanes();
    laneStart = m_rect.startLeft + startDir * (startWidth/2 + _index * startWidth);
    laneEnd = m_rect.endLeft + endDir * (endWidth/2 + _index * endWidth);


    /*
     *
     * TODO: save the lane connection mapping
     *
     *
     */

    LaneEndpoint startEndpoint;
    startEndpoint.node = m_startGate.node;
    startEndpoint.position = laneStart;
    startEndpoint.gate.direction = LANE_GATE::START;

    LaneEndpoint endEndpoint;
    endEndpoint.node = m_endGate.node;
    endEndpoint.position = laneEnd;
    endEndpoint.gate.direction = LANE_GATE::END;

    int directionType = p_way->getLaneDirectionType(_index);
    if(directionType==WAY_DIRECTION::BACKWARD)
    {
        LaneEndpoint temp = startEndpoint;
        startEndpoint = endEndpoint;
        endEndpoint = temp;

        startEndpoint.gate.direction = LANE_GATE::START;
        endEndpoint.gate.direction = LANE_GATE::END;
    }


    //
    Lane *lane = 0;
    if(_index>=m_lanes.size())
    {
        lane = new Lane(startEndpoint, endEndpoint, this);
        m_lanes.push_back(lane);
    }
    else
    {
        lane = m_lanes.at(_index);
        lane->setStartEndpoint(startEndpoint);
        lane->setEndEndpoint(endEndpoint);
    }
    lane->setIndex(_index);

    //
    lane->setDirectionType(directionType);

}

void Segment::computeRawRect()
{
    double backwardWidth_m = 3.5 * p_way->getForwardLanes();
    double forwardWidth_m = 3.5 * p_way->getBackwardLanes();

    Vector3d rightDir = getDirection().rotateRight();
    Position start = m_startGate.node->getPosition();
    Position end = m_endGate.node->getPosition();

    Rect rect;

    rect.startLeft = start - rightDir * backwardWidth_m;
    rect.endLeft = end - rightDir * backwardWidth_m;


    rect.startRight = start + rightDir * forwardWidth_m;
    rect.endRight = end + rightDir * forwardWidth_m;

    m_rect = rect;
}

void Segment::computeFinalRect()
{
    double w = 3.5;
    double backwardWidth_m = w * getBackwardLanes().size();
    double forwardWidth_m = w * getFowardLanes().size();


    Position start = m_startGate.node->getPosition();
    Position end = m_endGate.node->getPosition();

    // enable for intersections with offset
    start = m_startGate.position;
    end = m_endGate.position;



    // left and right are switched on purpose - the segment is the nearest segment in the direction from this segments view, the preX is the actual point of the bounding box
    Segment *preLeft = m_startGate.getNearestRightSegment();
    Segment *preRight = m_startGate.getNearestLeftSegment();
    if(preLeft)
    {
        Node *intersection = m_startGate.node;
        bool invertIn = !(preLeft->getEndGate()->node==intersection);
        bool invertOut = !(getStartGate()->node==intersection);

        Vector3d dir = computeMeanDirection(this, preLeft, invertOut, invertIn).rotateRight();
        double stretch = computeStretchFactor(getRotation(), computeMeanRotation(this, preLeft, invertOut, invertIn), invertOut);
        m_rect.startLeft =  start - dir * backwardWidth_m / stretch;

    }

    if(preRight)
    {
        Node *intersection = m_startGate.node;
        bool invertIn = !(preRight->getEndGate()->node==intersection);
        bool invertOut = !(getStartGate()->node==intersection);

        Vector3d dir = computeMeanDirection(this, preRight, invertOut, invertIn).rotateRight();
        double stretch = computeStretchFactor(getRotation(), computeMeanRotation(this, preRight, invertOut, invertIn), invertOut);

        m_rect.startRight =  start + dir * forwardWidth_m / stretch;
    }



    Segment *nextLeft = m_endGate.getNearestLeftSegment();
    Segment *nextRight = m_endGate.getNearestRightSegment();
    if(nextLeft)
    {
        Node *intersection = m_endGate.node;
        bool invertIn = !(nextLeft->getStartGate()->node==intersection);
        bool invertOut = !(getEndGate()->node==intersection);


        Vector3d dir = computeMeanDirection(this, nextLeft, invertOut, invertIn).rotateRight();
        double stretch = computeStretchFactor(getRotation(), computeMeanRotation(this, nextLeft, invertOut, invertIn), invertOut);

        m_rect.endLeft = end - dir * backwardWidth_m / stretch;
    }

    if(nextRight)
    {
        Node *intersection = m_endGate.node;

        bool invertIn = !(nextRight->getStartGate()->node==intersection);
        bool invertOut = !(getEndGate()->node==intersection);

        Vector3d dir = computeMeanDirection(this, nextRight, invertOut, invertIn).rotateRight();
        double stretch = computeStretchFactor(getRotation(), computeMeanRotation(this, nextRight, invertOut, invertIn), invertOut);
        m_rect.endRight = end + dir * forwardWidth_m / stretch;
    }


}

Position Segment::getCenter()
{
    Position center = (m_startGate.node->getPosition() + m_endGate.node->getPosition())/2;
    return center;
}

Vector3d Segment::getDirection()
{
    return (m_endGate.node->getPosition()-m_startGate.node->getPosition()).normed();
}

double Segment::getLength()
{
    return (m_endGate.node->getPosition()-m_startGate.node->getPosition()).norm();
}

double Segment::getRotation()
{
    return Math::computeRotation(m_startGate.node->getPosition(), m_endGate.node->getPosition());
}

Node* Segment::getOtherNode(Node *_node)
{
    Node *node = 0;
    if(_node==m_startGate.node)
        node = m_endGate.node;
    else if(_node==m_endGate.node)
        node = m_startGate.node;

    return node;
}

std::string Segment::toString()
{
    std::stringstream stream;
    stream << "way: " << p_way->getName() << " segment: " << m_startGate.node->getName() << " -> " << m_endGate.node->getName();

    return stream.str();
}

int Segment::getLaneIndex(Lane *_lane)
{
    for(unsigned int i=0; i<m_lanes.size(); i++)
    {
        if(m_lanes.at(i)==_lane)
            return i;
    }
    return -1;
}

Lane* Segment::getLane(int _index)
{
    if(_index<m_lanes.size())
        return m_lanes.at(_index);
    return 0;
}

std::vector<Lane*> Segment::getBackwardLanes()
{
    std::vector<Lane*> lanes;

    for(int i=m_lanes.size()-1; i>-1; i--) // backward lanes start outside -> reverse needed
    {
        Lane *lane = m_lanes.at(i);
        if(lane->getDirectionType()==WAY_DIRECTION::BACKWARD)
            lanes.push_back(lane);
    }

    return lanes;
}

std::vector<Lane*> Segment::getFowardLanes()
{
    std::vector<Lane*> lanes;

    for(unsigned int i=0; i<m_lanes.size(); i++)
    {
        Lane *lane = m_lanes.at(i);
        if(lane->getDirectionType()!=WAY_DIRECTION::BACKWARD)
            lanes.push_back(lane);
    }

    return lanes;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Segment::setStart(Node *_start)
{
    m_startGate.node = _start;
    m_startGate.position = _start->getPosition();

    m_startGate.node->addSegment(this);
}

void Segment::setEnd(Node *_end)
{
    m_endGate.node = _end;
    m_endGate.position = _end->getPosition();

    m_endGate.node->addSegment(this);
}

void Segment::setPredecessor(Segment *_predecessor)
{
    // TODO: for all lanes, remove all straight connections to the current predecessor (!=0)
    if(p_predecessor)
    {

    }

    p_predecessor = _predecessor;
}

void Segment::setSuccessor(Segment *_succsessor)
{
    // TODO: for all lanes, remove all straight connections to the current successor (!=0)
    if(p_successor)
    {

    }

    p_successor = _succsessor;
}

SegmentGate* Segment::getStartGate()
{
    return &m_startGate;
}

SegmentGate* Segment::getEndGate()
{
    return &m_endGate;
}

Way* Segment::getWay()
{
    return p_way;
}

Segment* Segment::getPredecessor()
{
    return p_predecessor;
}

Segment* Segment::getSuccessor()
{
    return p_successor;
}

std::vector<Lane*>& Segment::getLanes()
{
    return m_lanes;
}

Rect& Segment::getRect()
{
    return m_rect;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

Vector3d Segment::computeMeanDirection(Segment *_from, Segment *_to, bool _invertOut, bool _invertIn)
{
    Vector3d fromDir = _from->getDirection();
    Vector3d toDir = _to->getDirection();

    if(_invertIn)
        toDir = toDir * -1;
    if(_invertOut)
        toDir = toDir * -1;


    return (fromDir + toDir)/2;
}

double Segment::computeMeanRotation(Segment *_from, Segment *_to, bool _invertOut, bool _invertIn)
{
    double fromRot = _from->getRotation();
    double toRot = _to->getRotation();

    if(_invertIn)
        toRot = toRot - 180;
    if(_invertOut)
        fromRot = fromRot - 180;


    return (fromRot + toRot)/2;
}

double Segment::computeStretchFactor(double _angle0, double _angle1, bool _invertOut)
{
    // TODO: invertOut

    double angle = _angle0;
   // if(_invertOut)
   //     angle = angle - 180;

    double angleDifference = Math::getMinimumAngleDifference(angle, _angle1);
    double strechFactor = cos(Math::toRad(angleDifference));

    return strechFactor * strechFactor;
}

}
