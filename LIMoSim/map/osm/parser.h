#ifndef LIMOSIM_PARSER_H
#define LIMOSIM_PARSER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "LIMoSim/settings/filehandler.h"
#include "LIMoSim/settings/domelement.h"

namespace LIMoSim
{

class Parser
{
public:
    Parser();

    static std::vector<std::string> split(const std::string &_data, const std::string &_delimiter);
    static void info(const std::vector<std::string> &_data);

    bool startsWith(const std::string &_data, const std::string &_key);
    bool endsWith(const std::string &_data, const std::string &_key);

    std::string replace(const std::string &_data, const std::string _sub, const std::string &_replacement);

protected:
    FileHandler m_fileHandler;

    std::string m_buffer;

};

}

#endif // LIMOSIM_PARSER_H
