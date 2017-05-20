#include "domnode.h"
#include "domelement.h"

namespace LIMoSim
{

DOMNode::DOMNode(const std::string _nodeName, int _nodeType, DOMNode *_parent) :
    nodeName(_nodeName),
    nodeType(_nodeType),
    parentNode(_parent)
{

}

DOMNode::~DOMNode()
{
    for(unsigned int i=0; i<childNodes.size(); i++)
        delete childNodes.at(i);
}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void DOMNode::appendChild(DOMNode *_node)
{
    childNodes.push_back(_node);
}

DOMNode* DOMNode::firstChild()
{
    if(childNodes.size())
        return childNodes.at(0);
    return 0;
}

DOMNode* DOMNode::lastChild()
{
    if(childNodes.size())
        return childNodes.at(childNodes.size()-1);
    return 0;
}

bool DOMNode::hasAttribute(const std::string &_key)
{
    return (attributes.count(_key)>0);
}

bool DOMNode::hasAttributes()
{
    return (attributes.size()>0);
}

bool DOMNode::hasChildNodes()
{
    return (childNodes.size()>0);
}


DOMElement* DOMNode::toElement()
{
    return static_cast<DOMElement*>(this);
}

}
