#ifndef LIMOSIM_LANEGATE_H
#define LIMOSIM_LANEGATE_H

#include <map>
#include <vector>

namespace LIMOSIM
{
class Lane;


class LaneGate
{
public:
    LaneGate();

    Lane* getNextLane();
    bool isLinkedTo(Lane *_lane);

    void clearOut();


    void addIncomingLane(Lane *_lane, int _type);
    void removeIncoming(Lane *_lane); // called from the lane on all its out lanes


public:
    Lane *lane;
    std::map<Lane*,int> in; // turn type is not unique
    std::map<int,Lane*> out; // turn type is unique
    int direction;
};

}

#endif // LIMOSIM_LANEGATE_H
