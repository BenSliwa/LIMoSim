#ifndef LIMOSIM_MAPOBJECT_H
#define LIMOSIM_MAPOBJECT_H

#include <iostream>
#include <sstream>
#include "mapdefinitions.h"
#include "map.h"

namespace LIMOSIM
{

class MapObject
{
public:
    MapObject(const std::string &_id, int _type, const std::string &_name = "");

    virtual std::string toString() = 0;
    void info();

    // property accessors
    void setId(int _id);
    virtual void setType(int _type);
    void setName(const std::string &_name);

    std::string getId();
    int getType();
    std::string getName();

protected:
    std::string m_id;
    int m_type;
    std::string m_name;

    Map *p_map;
};

}


#endif // LIMOSIM_MAPOBJECT_H
