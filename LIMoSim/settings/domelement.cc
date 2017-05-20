#include "domelement.h"

namespace LIMoSim
{

DOMElement::DOMElement(const std::string &_tagName) : DOMNode("element", NODE_TYPES::ELEMENT_NODE),
    tagName(_tagName)
{

}

DOMElement::~DOMElement()
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void DOMElement::setAttribute(const std::string &_key, const Variant &_value)
{
    attributes[_key] = _value;
}

Variant DOMElement::getAttribute(const std::string &_key)
{
    return attributes[_key];
}

DOMElement* DOMElement::getFirstChildWithTag(const std::string &_tagName)
{
    DOMElement *element = 0;

    for(unsigned int i=0; i<childNodes.size(); i++)
    {
        DOMElement *child = childNodes.at(i)->toElement();
        if(child->tagName==_tagName)
        {
            element = child;
            break;
        }
    }

    return element;
}

std::string DOMElement::toString(int _level)
{
    std::stringstream stream;

    for(int l=0; l<_level; l++)
        stream << "\t";

    stream << "<" << tagName; // opening tag
    std::map<std::string, Variant>::iterator attribute;
    for(attribute=attributes.begin(); attribute!=attributes.end(); attribute++)
    {
        std::string key = attribute->first;
        std::string value = attribute->second.toString();

        stream << " " << key << "=\"" << value << "\"";
    }
    if(childNodes.size())
    {
        stream << ">";

        // TODO: children
        for(unsigned int i=0; i<childNodes.size(); i++)
        {
            DOMElement *currentChild = childNodes.at(i)->toElement();
            stream << "\n" << currentChild->toString(_level+1);
        }

        stream << "\n";
        for(int l=0; l<_level; l++)
            stream << "\t";
        stream  << "</" << tagName << ">"; // closing tag
    }
    else
    {
        stream << "/>";
    }


    return stream.str();
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/

}
