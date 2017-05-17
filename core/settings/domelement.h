#ifndef LIMOSIM_DOMELEMENT_H
#define LIMOSIM_DOMELEMENT_H

#include "domnode.h"

// https://www.w3schools.com/xml/dom_nodes.asp
// https://www.w3schools.com/xml/dom_element.asp

namespace LIMOSIM
{

class DOMElement : public DOMNode
{
public:
    DOMElement(const std::string &_tagName = "");
    virtual ~DOMElement();

    void setAttribute(const std::string &_key, const Variant &_value);
    Variant getAttribute(const std::string &_key);

    DOMElement* getFirstChildWithTag(const std::string &_tagName);

    std::string toString(int _level = 0);




public:
    std::string tagName;
    std::string text;



};

}

#endif // LIMOSIM_DOMELEMENT_H
