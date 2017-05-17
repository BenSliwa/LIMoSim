#include "nodegrid.h"
#include "map.h"

namespace LIMoSim
{

NodeGrid::NodeGrid()
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void NodeGrid::init(int _x, double _xDistance_m, int _y, double _yDistance_m)
{
    Map *map = Map::getInstance();

    double xOffset = 0;
    double yOffset = 0;

    m_columns = _x;
    m_rows = _y;

    for(int y=0; y<m_rows; y++)
    {
        for(int x=0; x<m_columns; x++)
        {
            double xPos = x * _xDistance_m + xOffset;
            double yPos = y * _yDistance_m + yOffset;

            Node *node = map->createNode(Position(xPos, yPos));
            m_nodes[std::pair<int,int>(x,y)] = node;
        }
    }

    // horizontal ways
    for(int y=0; y<m_rows; y++)
    {
        Way *way = map->createWay();
        for(int x=0; x<m_columns; x++)
        {
            Node *node = getNode(x,y);
            way->addNode(node);
        }
        way->linkLanes();
    }

    // vertical ways
    for(int x=0; x<m_columns; x++)
    {
        Way *way = map->createWay();
        for(int y=0; y<m_rows; y++)
        {
            Node *node = getNode(x,y);
            way->addNode(node);
        }
        way->linkLanes();
    }


}

Node* NodeGrid::getNode(int _x, int _y)
{
    return m_nodes[std::pair<int,int>(_x,_y)];
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/



/*************************************
 *           PRIVATE METHODS         *
 ************************************/

}
