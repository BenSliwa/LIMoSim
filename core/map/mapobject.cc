#include "mapobject.h"


namespace LIMOSIM
{

MapObject::MapObject(const std::string &_id, int _type, const std::string &_name) :
    m_id(_id),
    m_type(_type),
    m_name(_name),
    p_map(Map::getInstance())
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void MapObject::info()
{
    std::cout << toString() << std::endl;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void MapObject::setId(int _id)
{
    m_id = _id;
}

void MapObject::setType(int _type)
{
    m_type = _type;
}

void MapObject::setName(const std::string &_name)
{
    m_name = _name;
}

std::string MapObject::getId()
{
    return m_id;
}

int MapObject::getType()
{
    return m_type;
}

std::string MapObject::getName()
{
    return m_name;
}

}
