#include "LIMoSim/location/math_.h"
#include "node.h"
#include "lane.h"
#include "segment.h"
#include "way.h"
#include "trafficsignal.h"
#include <limits>
#include <algorithm>

namespace LIMoSim
{

Node::Node(const Position &_position, const std::string &_id, int _type, const std::string &_name) :
    MapObject(_id, _type, _name),
    m_position(_position),
    m_signal(0)
{
    if(m_name=="")
    {
        std::stringstream stream;
        stream << "N" << m_id;
        m_name = stream.str();
    }
}

Node::~Node()
{
    if(m_signal)
        delete m_signal;

    std::map<std::pair<Lane*,Lane*>,Lane*>::iterator it;
    for(it=m_connectionLanes.begin(); it!=m_connectionLanes.end(); it++)
    {
        Lane *lane = it->second;
        delete lane;
    }
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Node::addSegment(Segment *_segment)
{
    if(getSegmentIndex(_segment)==-1)
        m_segments.push_back(_segment);
}

void Node::removeSegment(Segment *_segment)
{
    int index = getSegmentIndex(_segment);
    if(index>-1)
        m_segments.erase(m_segments.begin() + index);
}

int Node::getSegmentIndex(Segment *_segment)
{
    int index = -1;
    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        if(m_segments.at(i)==_segment)
        {
            index = i;
            break;
        }
    }

    return index;
}

bool Node::isReachableNeighbor(Node *_node)
{
    bool reachable = false;

    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);
        if(segment->getOtherNode(this)==_node)
        {
            int type = checkSegmentDirection(segment);
            if(type!=WAY_DIRECTION::BACKWARD)
            {
                reachable = true;
                break;
            }
        }


    }

    return reachable;
}

int Node::checkSegmentDirection(Segment *_segment)
{
    int type = WAY_DIRECTION::BIDIRECTIONAL;
    int directionType = _segment->getWay()->getDirectionType();
    if(_segment->getStartGate()->node==this) // movement with the segment direction
        type = directionType;
    else // movement against the segment direction
    {
        if(directionType==WAY_DIRECTION::FORWARD)
            type = WAY_DIRECTION::BACKWARD;
        else if(directionType==WAY_DIRECTION::BACKWARD)
            type = WAY_DIRECTION::FORWARD;
    }

    return type;
}

Segment* Node::getOtherSegment(Segment *_segment)
{
    Segment *segment = 0;

    if(m_segments.size()==2)
    {
        if(_segment==m_segments.at(0))
            segment = m_segments.at(1);
        else if(_segment==m_segments.at(1))
            segment = m_segments.at(0);
    }

    return segment;
}


bool Node::isIntersectionNode()
{
    int s = m_segments.size();
    if(s==1 || s>2)
        return true;
    return false;
}

void Node::updateConnectionLanes()
{
    // loop over all lanes and delete them if their linked lanes are not connected anymore
    std::map<std::pair<Lane*,Lane*>,Lane*>::iterator it;
    for(it=m_connectionLanes.begin(); it!=m_connectionLanes.end();)
    {
        Lane *out = it->first.first;
        Lane *lane = it->second;

        if(!out->getEndEndpoint()->gate.isLinkedTo(lane))
        {
            m_connectionLanes.erase(it++);
            delete lane;
        }
        else
            ++it;
    }
}

void Node::linkSegments(bool _loopback)
{
    std::vector<Segment*> incoming = getIncomingSegments();
    std::vector<Segment*> outgoing = getOutgoingSegments();
    for(unsigned int i=0; i<incoming.size(); i++)
    {
        Segment *from = incoming.at(i);
        Node *previous = from->getOtherNode(this);

        // angle
        double inAngle = Math::computeRotation(previous->getPosition(), this->getPosition());

        // clear gates
        from->clearLaneLinks(this);
        SegmentGate *segmentGate = from->getGateForNode(this);
        segmentGate->segments.clear();

        for(unsigned int j=0; j<outgoing.size(); j++)
        {
            Segment *to = outgoing.at(j);
            if(from!=to)
            {
                Node *next = to->getOtherNode(this);
                double outAngle = Math::computeRotation(this->getPosition(), next->getPosition());
                double angleDelta = Math::getAngleDifference(inAngle, outAngle);


                /*  TODO:
                 *  - assume a straight connection for segments with minimum delta && delta<90
                 *
                 */

                int turnType = getTurnType(angleDelta);
                segmentGate->segments[turnType] = to;
            }
        }

        // update the gate
        from->updateGatePosition(segmentGate);

        from->computeRawRect();
        from->computeFinalRect();

        // connect the segments from the saved gate settings
        std::map<int,Segment*>::iterator it;
        for(it=segmentGate->segments.begin(); it!=segmentGate->segments.end(); it++)
        {
            int type = it->first;
            Segment *segment = it->second;


            bool direct = true;
            if(m_segments.size()>2)
                direct = false;
            if(type==TURN::RIGHT || type==TURN::SHARP_RIGHT || type==TURN::LEFT || type==TURN::SHARP_LEFT)
                direct = false;


            if(type!=TURN::NONE)
                from->connectTo(segment, this, type, direct); // TODO: isIntersectionNode()
        }

    }





    //
    if(_loopback && m_segments.size()==1) // loopback
    {
        if(m_incoming.size() && m_outgoing.size())
        {
            Segment *segment = m_incoming.at(0).segment;
            std::vector<Lane*> forwardLanes = segment->getFowardLanes();
            std::vector<Lane*> backwardLanes = segment->getBackwardLanes();

            int s = std::min(forwardLanes.size(), backwardLanes.size());
            for(unsigned int i=0; i<s; i++)
            {
                Lane *out = forwardLanes.at(i);
                Lane *in = backwardLanes.at(i);

                LaneGate *outGate = out->getGateForNode(this);
                LaneGate *inGate = in->getGateForNode(this);

                outGate->out[TURN::THROUGH] = in;
                inGate->out[TURN::THROUGH] = out;

            }
        }
    }

    updateConnectionLanes();
    updateTrafficSignals();
}

int Node::getTurnType(double _angleDelta)
{
    int type = TURN::NONE;

    double delta = std::abs(_angleDelta);

    double slight = 25;
    double turn = 50;
    double sharp = 100;
    double limit = 180;

    if(delta<slight)
        type = TURN::THROUGH;
    else if(delta>limit)
        type = TURN::NONE;
    else if(Math::inInterval(_angleDelta, slight, turn))
        type = TURN::SLIGHT_LEFT;
    else if(Math::inInterval(_angleDelta, turn, sharp))
        type = TURN::LEFT;
    else if(Math::inInterval(_angleDelta, sharp, limit))
        type = TURN::SHARP_LEFT;
    else if(Math::inInterval(_angleDelta, -turn, -slight))
        type = TURN::SLIGHT_RIGHT;
    else if(Math::inInterval(_angleDelta, -sharp, -turn))
        type = TURN::RIGHT;
    else if(Math::inInterval(_angleDelta, -limit, -sharp))
        type = TURN::SHARP_RIGHT;


    return type;
}

std::string Node::getTurnTypeDescription(int _type)
{
    std::map<int,std::string> types;
    types[TURN::NONE] = "none";
    types[TURN::THROUGH] = "through";
    types[TURN::SLIGHT_LEFT] = "slight left";
    types[TURN::LEFT] = "left";
    types[TURN::SHARP_LEFT] = "sharp left";
    types[TURN::SLIGHT_RIGHT] = "slight right";
    types[TURN::RIGHT] = "right";
    types[TURN::SHARP_RIGHT] = "sharp right";

    return types[_type];
}

Lane* Node::getConnectionLane(Lane *_from, Lane *_to)
{
    std::pair<Lane*,Lane*> connection(_from, _to);
    if(m_connectionLanes.count(connection))
        return m_connectionLanes[connection];
    else
    {
        // create a new connection lane
        LaneEndpoint outEndpoint = *_from->getEndpointForNode(this);
        LaneEndpoint inEndpoint = *_to->getEndpointForNode(this);

        Lane *connectionLane = new Lane(outEndpoint, inEndpoint, 0);
        connectionLane->setIsConnectionLane(true);

        m_connectionLanes[connection] = connectionLane;

        return connectionLane;
    }

}

void Node::updateDestinations()
{
    // update this node
    computeDestinations();

    // update the neighbors
    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Node *neighbor = m_segments.at(i)->getOtherNode(this);
        if(neighbor)
        {
            neighbor->computeDestinations();
        }
    }

}

void Node::computeDestinations()
{
    m_outgoing.clear();
    m_incoming.clear();

    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);

        int type = checkSegmentDirection(segment);

        /*
         *  TODO: check segment direction change
         *
         *
         */


        Node *previous = this;
        Node *node = segment->getOtherNode(previous);

        DestinationEntry dest;
        dest.neighbor = node;
        dest.way = segment->getWay();
        dest.cost = (node->getPosition()-previous->getPosition()).norm();
        dest.segment = segment;

        while(!node->isIntersectionNode())
        {
            // if the node is no intersection node, it has exactly two segments
            // -> get the other segment
            std::vector<Segment*>& segments = node->getSegments();
            if(segments.at(0)==segment)
                segment = segments.at(1);
            else
                segment = segments.at(0);

            previous = node;
            node = segment->getOtherNode(previous);

            dest.cost += (node->getPosition()-previous->getPosition()).norm();

            if(node==this) // circle way detected
            {
                /*
                 *  TODO: maybe add that link as well
                 *
                 */


                return;
            }
        }

        dest.destination = node;


        if(type==WAY_DIRECTION::FORWARD || type==WAY_DIRECTION::BIDIRECTIONAL)
            m_outgoing.push_back(dest);
        if(type==WAY_DIRECTION::BACKWARD || type==WAY_DIRECTION::BIDIRECTIONAL)
            m_incoming.push_back(dest);
    }
}

bool Node::knowsDestination(Node *_node)
{
    for(unsigned int i=0; i<m_outgoing.size(); i++)
    {
        DestinationEntry entry = m_outgoing.at(i);
        if(entry.destination==_node)
            return true;
    }
    return false;
}

DestinationEntry Node::getBestDestinationEntry(Node *_node, Node *_avoid, bool _incoming)
{

    // find the entry with the lowest cost to the destination
    DestinationEntry entry;
    entry.isValid = false;
    entry.cost = std::numeric_limits<double>::infinity();

    std::vector<DestinationEntry> entries;
    if(_incoming)
        entries = m_incoming;
    else
        entries = m_outgoing;

    for(unsigned int i=0; i<entries.size(); i++)
    {
        DestinationEntry currentEntry = entries.at(i);
        bool avoid = false;
        if(!_incoming && currentEntry.neighbor==_avoid)
            avoid = true;
        else if(_incoming && _node==_avoid)
            avoid = true;

        if(currentEntry.destination==_node && currentEntry.cost<entry.cost && !avoid)
            entry = currentEntry;

        if(currentEntry.neighbor)
            std::cout << "possible: " << this->getName() << " -> " << currentEntry.neighbor->getName() << "\t" << _incoming << std::endl;
    }

    return entry;
}

void Node::updateTrafficSignals()
{
    std::vector<TrafficSignal*> trafficSignals;
    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);
        Node *node = segment->getOtherNode(this);
        TrafficSignal *signal = node->getTrafficSignal();

        if(!signal)
        {
            Segment *otherSegment = node->getOtherSegment(segment);
            if(otherSegment)
            {
                node = otherSegment->getOtherNode(node);
                signal = node->getTrafficSignal();
            }
        }

        if(signal)
        {
            double angle = Math::computeRotation(this->getPosition(), node->getPosition());
            signal->setWayAngle(angle);
            trafficSignals.push_back(signal);
        }
    }

    // set the first signal depending on the road direction
    if(trafficSignals.size()>1)
    {
        // set all other signals depending on the first one
        for(unsigned int i=0; i<trafficSignals.size(); i++)
        {
            TrafficSignal *signal = trafficSignals.at(i);
            signal->setStateWithAngle(trafficSignals.at(0)->getWayAngle());
        }
    }


}

std::vector<Segment*> Node::getOutgoingSegments()
{
    std::vector<Segment*> segments;

    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);
        int type = checkSegmentDirection(segment);
        if(type==WAY_DIRECTION::FORWARD || type==WAY_DIRECTION::BIDIRECTIONAL)
            segments.push_back(segment);
    }

    return segments;
}

std::vector<Segment*> Node::getIncomingSegments()
{
    std::vector<Segment*> segments;

    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);
        int type = checkSegmentDirection(segment);
        if(type==WAY_DIRECTION::BACKWARD || type==WAY_DIRECTION::BIDIRECTIONAL)
            segments.push_back(segment);
    }

    return segments;
}


std::string Node::toString()
{
    std::stringstream stream;

    stream << getName() << " " << m_position.toString() << ": [";

    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);
        Node *node = segment->getOtherNode(this);
        stream << node->getName() << ":" << checkSegmentDirection(segment);

        if(i<m_segments.size()-1)
            stream << " ,";
    }
    stream << "]";

    return stream.str();
}


/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Node::setPosition(const Position &_position)
{
    m_position = _position;

    updateDestinations();
}

void Node::setTrafficSignal(TrafficSignal *_signal)
{
    m_signal = _signal;
}

std::vector<Segment*>& Node::getSegments()
{
    return m_segments;
}

std::vector<DestinationEntry>& Node::getOutgoingConnections()
{
    return m_outgoing;
}

std::vector<DestinationEntry>& Node::getIncomingConnections()
{
    return m_incoming;
}

std::map<std::pair<Lane*,Lane*>,Lane*>& Node::getConnectionLanes()
{
    return m_connectionLanes;
}

Position Node::getPosition()
{
    return m_position;
}

TrafficSignal* Node::getTrafficSignal()
{
    return m_signal;
}

}
