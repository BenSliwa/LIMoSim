#include "parser.h"

namespace LIMoSim
{

Parser::Parser()
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

std::vector<std::string> Parser::split(const std::string &_data, const std::string &_delimiter)
{
    std::vector<std::string> list;

    std::string data = _data;

    size_t pos = 0;
    std::string token;
    while ((pos = data.find(_delimiter)) != std::string::npos) {
        token = data.substr(0, pos);
        list.push_back(token);
        data.erase(0, pos + _delimiter.length());
    }
    list.push_back(data);

    return list;
}

void Parser::info(const std::vector<std::string> &_data)
{
    for(unsigned int i=0; i<_data.size(); i++)
    {
        std::cout << _data.at(i) << std::endl;
    }
}

bool Parser::startsWith(const std::string &_data, const std::string &_key)
{
    if(_data.size()>=_key.size())
    {
        for(unsigned int i=0; i<_key.size(); i++)
        {
            if(_data.at(i)!=_key.at(i))
                return false;
        }
        return true;
    }
    else
        return false;
}

bool Parser::endsWith(const std::string &_data, const std::string &_key)
{
    int s = _key.size();
    if(_data.size()>=s)
    {
        for(unsigned int i=0; i<s; i++)
        {
            int index = _data.size() - s + i;
            if(_data.at(index)!=_key.at(i))
                return false;
        }
        return true;
    }
    else
        return false;

}

std::string Parser::replace(const std::string &_data, const std::string _sub, const std::string &_replacement)
{
    std::string data = _data;
    for( size_t pos = 0; ; pos += _replacement.length() )
    {
        pos = data.find( _sub, pos );
        if( pos == std::string::npos )
            break;

        data.erase( pos, _sub.length() );
        data.insert( pos, _replacement );
    }

    return data;
}

}
