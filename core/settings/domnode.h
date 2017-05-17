#ifndef DOMNODE_H
#define DOMNODE_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "variant.h"
#include "domdefinitions.h"

// https://www.w3schools.com/xml/dom_node.asp

namespace LIMoSim
{
class DOMElement;

class DOMNode
{
public:
    DOMNode(const std::string _nodeName, int _nodeType, DOMNode *_parent = 0);
    virtual ~DOMNode();

    void appendChild(DOMNode *_node);
    DOMNode* firstChild();
    DOMNode* lastChild();

    bool hasAttribute(const std::string &_key);
    bool hasAttributes();
    bool hasChildNodes();

    //
    DOMElement* toElement();

public:
    std::vector<DOMNode*> childNodes;
    std::map<std::string, Variant> attributes;

    std::string nodeName;
    int nodeType;

    DOMNode *parentNode;
};

}

#endif // DOMNODE_H
