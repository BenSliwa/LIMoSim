#include "xmlparser.h"
#include "core/settings/domdefinitions.h"

namespace LIMoSim
{

XMLParser::XMLParser() : Parser()
{

}

DOMElement* XMLParser::parse(const std::string &_path)
{
    std::string data = m_fileHandler.listToString(m_fileHandler.read(_path));

    return parseData(data);
}

DOMElement* XMLParser::parseData(const std::string &_data)
{
    //
    std::vector<std::string> nodes;
    for(unsigned int i=0; i<_data.size(); i++)
    {
        char c = _data.at(i);
        if(c=='<')
        {
            if(m_buffer!="")
                nodes.push_back(m_buffer);
            m_buffer = c;
        }
        else if(c=='>')
        {
            m_buffer += c;
            nodes.push_back(m_buffer);
            m_buffer = "";
        }
        else
            m_buffer += c;
    }



    //
    DOMElement *element = parseNode(nodes);
    return element;
}

DOMElement* XMLParser::parseNode(std::vector<std::string> &_data)
{
    DOMElement *element = 0; // here: assuming the first element is a start node
    std::string data = _data.at(0);
    _data.erase(_data.begin());
    element = parseAttributes(data);

    while(_data.size()>0)
    {
        std::string data = _data.at(0);

        int type = getType(data);
        if(type==DOM::START_TAG)
        {
            element->appendChild(parseNode(_data));
        }
        else if(type==DOM::INLINE_TAG)
        {
            element->appendChild(parseAttributes(data));
        }
        else if(type==DOM::END_TAG)
        {
            _data.erase(_data.begin());
            return element;
        }
        else // text node
        {
            element->text = data;

            //std::cout << element->tagName << "\t" << element->text << std::endl;
        }

        _data.erase(_data.begin());
    }

    return element;
}

DOMElement* XMLParser::parseAttributes(const std::string &_data)
{
    DOMElement *element = new DOMElement();

    // extract the payload
    std::string data = _data;
    data = replace(data, "<", "");
    data = replace(data, "/>", "");
    data = replace(data, ">", "");

    // get the tag name and parse the attributes
    std::vector<std::string> items = split(data, " ");
    if(items.size()>0)
    {
        element->tagName = items.at(0);

        for(unsigned int i=1; i<items.size(); i++)
            parseAttribute(items.at(i), element);
    }

    return element;
}

void XMLParser::parseAttribute(const std::string &_data, DOMElement *_element)
{
    std::vector<std::string> items = split(_data, "=");
    {
        if(items.size()==2) // key, value
        {
            std::string key = items.at(0);
            std::string value = items.at(1);
            value = replace(value, "\"", "");
            value = replace(value, "'", "");

            _element->setAttribute(key, value);
        }
    }
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/


int XMLParser::getType(const std::string &_data)
{
    int type = DOM::CONTENT;
    if(endsWith(_data, "/>"))
        type = DOM::INLINE_TAG;
    else if(startsWith(_data, "</"))
        type = DOM::END_TAG;
    else if(endsWith(_data, ">"))
        type = DOM::START_TAG;

    return type;
}

}

