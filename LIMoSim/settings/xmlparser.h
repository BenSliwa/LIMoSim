#ifndef LIMOSIM_XMLPARSER_H
#define LIMOSIM_XMLPARSER_H

#include "filehandler.h"
#include "LIMoSim/map/osm/parser.h"
#include "LIMoSim/settings/domelement.h"

namespace LIMoSim
{

class XMLParser : public Parser
{
public:
    XMLParser();

    DOMElement* parse(const std::string &_path);
    DOMElement* parseData(const std::string &_data);
    DOMElement* parseNode(std::vector<std::string> &_data);
    DOMElement* parseAttributes(const std::string &_data);
    void parseAttribute(const std::string &_data, DOMElement *_element);

private:
    int getType(const std::string &_data);
};

}

#endif // LIMOSIM_XMLPARSER_H
