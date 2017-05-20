#ifndef NODEGRID_H
#define NODEGRID_H

#include "node.h"
#include "way.h"
#include <map>

namespace LIMoSim
{

class NodeGrid
{
public:
    NodeGrid();

    void init(int _x, double _xDistance_m, int _y, double _yDistance_m);

    Node* getNode(int _x, int _y);

private:
    std::map<std::pair<int,int>,Node*> m_nodes;
    int m_columns;
    int m_rows;
};

}


#endif // NODEGRID_H
