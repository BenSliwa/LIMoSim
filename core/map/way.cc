#include "way.h"

namespace LIMoSim
{


Way::Way(const std::string &_id, int _type, const std::string &_name) :
    MapObject(_id, _type, _name),
    m_lanes(2),
    m_forwardLanes(1),
    m_bidirectionalLanes(0),
    m_backwardLanes(1),
    m_isOneway(0),
    m_maxSpeed_mps(50/3.6)
{
    if(m_name=="")
    {
        std::stringstream stream;
        stream << "W" << m_id;
        m_name = stream.str();
    }
}

Way::~Way()
{
    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);
        delete segment;
    }


}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

int Way::getPathDirection(Node *_from, Node *_direction)
{
    int fromIndex = getNodeIndex(_from);
    int toIndex = getNodeIndex(_direction);

    int direction = WAY_DIRECTION::FORWARD;
    if(fromIndex>toIndex)
        direction = WAY_DIRECTION::BACKWARD;

    return direction;
}


/*************************************
 *             NODE METHODS          *
 ************************************/

Segment* Way::addNode(Node *_node)
{
    Segment *segment = 0;
    if(m_nodes.size())
    {
        // create a node
        Node *previous = m_nodes.at(m_nodes.size()-1);

        // create a segment and link it to its nodes
        segment = new Segment(previous, _node, this);
        previous->updateDestinations();
        _node->updateDestinations();

        if(m_segments.size())
        {
            Segment *predecessor = m_segments.at(m_segments.size()-1);
            segment->setPredecessor(predecessor);
            predecessor->setSuccessor(segment);
        }

        segment->computeRawRect();
        segment->computeFinalRect();

        m_segments.push_back(segment);
    }

    m_nodes.push_back(_node);




    return segment;
}

void Way::removeNode(Node *_node)
{
    int index = getNodeIndex(_node);
    if(index>-1)
    {
        Node *predecessor = getNode(index-1);
        Node *successor = getNode(index+1);

        // update the segments
        Segment *previousSegment = getSegment(predecessor, _node);
        Segment *nextSegment = getSegment(_node, successor);
        if(previousSegment && nextSegment) // link the segments
        {
            int index = getSegmentIndex(previousSegment);
            if(index>-1)
            {
                Node *node = previousSegment->getStartGate()->node;
                nextSegment->setStart(node);
                previousSegment->setEnd(node);

                m_segments.erase(m_segments.begin() + index);
                delete previousSegment;
            }
        }
        else if(previousSegment) // the deleted segment was an end segment
        {
            int index = getSegmentIndex(previousSegment);
            m_segments.erase(m_segments.begin() + index);
            delete previousSegment;
        }
        else if(nextSegment) // the deleted segment was a start segment
        {
            int index = getSegmentIndex(nextSegment);
            m_segments.erase(m_segments.begin() + index);
            delete nextSegment;
        }


        m_nodes.erase(m_nodes.begin() + index);

        linkSegments();
    }

}

int Way::getNodeIndex(Node *_node)
{
    int index = -1;
    for(unsigned int i=0; i<m_nodes.size(); i++)
    {
        if(m_nodes.at(i)==_node)
        {
            index = i;
            break;
        }
    }

    return index;
}

Node* Way::getNode(int _index)
{
    Node *node = 0;
    if(_index>-1 && _index<m_nodes.size())
        node = m_nodes.at(_index);

    return node;
}

Segment* Way::insertNode(Segment *_segment)
{
    Segment *predecessor = _segment->getPredecessor();

    Node *start = _segment->getStartGate()->node;
    Node *end = _segment->getEndGate()->node;
    Node *node = p_map->createNode(_segment->getCenter());
    start->removeSegment(_segment);

    // create a new segment which ends in the new node
    Segment *segment = new Segment(start, node, this);
    segment->setPredecessor(predecessor);
    segment->setSuccessor(_segment);

    // update the originator segment
    _segment->setPredecessor(segment);
    _segment->setStart(node);

    int index = getSegmentIndex(_segment);
    m_segments.insert(m_segments.begin() + index, segment);
    m_nodes.insert(m_nodes.begin() + index + 1, node);

    node->computeDestinations();
    start->computeDestinations();
    end->computeDestinations();

    linkSegments();

    return segment;


    // TODO: update destinations and neighbors
}

/*************************************
 *           SEGMENT METHODS         *
 ************************************/

void Way::removeSegment(Segment *_segment)
{
    int index = getSegmentIndex(_segment);
    if(index>-1)
    {
        // remove the segment way
        Segment *predecessor = _segment->getPredecessor();
        Segment *successor = _segment->getSuccessor();

        if(!predecessor)
        {
            m_nodes.erase(m_nodes.begin());
        }
        if(!successor)
        {
            m_nodes.erase(m_nodes.begin() + m_nodes.size()-1);
        }

        //
        if(predecessor)
            predecessor->setSuccessor(0);
        if(successor)
            successor->setPredecessor(0);

        //
        delete _segment;
        m_segments.erase(m_segments.begin() + index);
    }

    linkSegments();


    // TODO: delete the way if no nodes remain
    // TODO: split the way if the segment is not an end node

}

int Way::getSegmentIndex(Segment *_segment)
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

Segment* Way::getSegment(int _index)
{
    Segment *segment = 0;
    if(_index>-1 && _index<m_segments.size())
        segment = m_segments.at(_index);

    return segment;
}

Segment* Way::getSegment(Node *_start, Node *_end)
{
    Segment *segment = 0;
    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *currentSegment = m_segments.at(i);
        if(currentSegment->getStartGate()->node==_start && currentSegment->getEndGate()->node==_end)
        {
            segment = currentSegment;
            break;
        }
    }

    return segment;
}


void Way::linkSegments()
{
    /* TODO: is called after segments have been added or removed,
     * however it would be better to only update the respective segements in those methods
     *
     */
    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);

        Segment *pre = getSegment(i-1);
        Segment *post = getSegment(i+1);

        segment->setPredecessor(pre);
        segment->setSuccessor(post);

    }

}

void Way::linkLanes()
{
    for(unsigned int i=0; i<m_segments.size(); i++)
    {
        Segment *segment = m_segments.at(i);
        segment->linkLanes();
    }

}

bool Way::isBidirectional()
{
    bool bidirectional = true;
    if(m_isOneway)
        bidirectional = false;

    return bidirectional;
}

int Way::getDirectionType()
{
    int type = 0;
    int forward = m_lanes - m_backwardLanes;
    int backward = m_lanes - m_forwardLanes;

    if(forward>0 && backward>0)
        type = WAY_DIRECTION::BIDIRECTIONAL;
    else if(forward>0)
        type = WAY_DIRECTION::FORWARD;
    else if(backward>0)
        type = WAY_DIRECTION::BACKWARD;

    return type;
}

int Way::getLaneDirectionType(int _index)
{
    int type = WAY_DIRECTION::FORWARD;

    if(_index<m_backwardLanes)
        type = WAY_DIRECTION::BACKWARD;
    else if(_index<m_backwardLanes+m_bidirectionalLanes)
        type = WAY_DIRECTION::BIDIRECTIONAL;

    return type;
}

double Way::getLength()
{
    double length_m = 0;
    if(m_nodes.size()>1)
    {
        Node *lastNode = m_nodes.at(0);
        for(unsigned int i=1; i<m_nodes.size(); i++)
        {
            Node *node = m_nodes.at(i);
            length_m += (node->getPosition()-lastNode->getPosition()).norm();

            lastNode = node;
        }
    }

    return length_m;
}

double Way::getForwardWidth()
{
    double laneWidth_m = 3.5;
    return getForwardLanes() * laneWidth_m;
}

double Way::getBackwardWidth()
{
    double laneWidth_m = 3.5;
    return getBackwardLanes() * laneWidth_m;
}

void Way::setLanes(int _total, int _forward, int _bidirectional, int _backward)
{
    m_lanes = _total;
    m_forwardLanes = _forward;
    m_bidirectionalLanes = _bidirectional;
    m_backwardLanes = _backward;
}

std::string Way::toString()
{
    std::stringstream stream;
    /*
    stream << "W" << m_id;*/

    //stream << "*******************" << std::endl;
    for(unsigned int i=0; i<m_nodes.size(); i++)
    {
        stream << m_nodes.at(i)->toString();
        if(i<m_nodes.size()-1)
            stream << " -> ";
    }

    return stream.str();
}


/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void Way::setType(int _type)
{
    MapObject::setType(_type);

    // load the default settings
    if(_type==WAY_TYPE::PRIMARY_HIGHWAY)
    {
        m_lanes = 2;
        m_forwardLanes = 1;
        m_backwardLanes = 1;
    }
    else if(_type==WAY_TYPE::RESIDENTIAL)
    {
        m_lanes = 2;
        m_forwardLanes = 1;
        m_backwardLanes = 1;
    }
}

void Way::setLanes(int _lanes)
{
    if(m_lanes!=_lanes)
    {
        // TODO: if the new value is lower, some lanes have to be deleted


        linkLanes();
    }



    m_lanes = _lanes;
}

void Way::setForwardLanes(int _lanes)
{
    m_forwardLanes = _lanes;
}

void Way::setBidirectionalLanes(int _lanes)
{
    m_bidirectionalLanes = _lanes;
}

void Way::setBackwardLanes(int _lanes)
{
    m_backwardLanes = _lanes;
}

void Way::setIsOneway(bool _oneway)
{
    m_isOneway = _oneway;
}

void Way::setMaxSpeed(double _maxSpeed)
{
    m_maxSpeed_mps = _maxSpeed;
}

std::vector<Node*>& Way::getNodes()
{
    return m_nodes;
}

std::vector<Segment*>& Way::getSegments()
{
    return m_segments;
}

int Way::getLanes()
{
    return m_lanes;
}

int Way::getForwardLanes()
{
    return m_forwardLanes;
}

int Way::getBidirectionalLanes()
{
    return m_bidirectionalLanes;
}

int Way::getBackwardLanes()
{
    return m_backwardLanes;
}

bool Way::isOneway()
{
    return m_isOneway;
}

double Way::getMaxSpeed()
{
    return m_maxSpeed_mps;
}



}
