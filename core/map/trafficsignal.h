#ifndef TRAFFICSIGNAL_H
#define TRAFFICSIGNAL_H

#include "node.h"
#include "core/sim/eventhandler.h"

namespace LIMOSIM
{

namespace TRAFFIC_SIGNAL
{
    enum{
        GREEN,
        YELLOW,
        RED,
        RED_YELLOW
    };
}


class TrafficSignal : public EventHandler
{
public:
    TrafficSignal(Node *_node);
    ~TrafficSignal();

    void setStateWithAngle(double _reference);
    double getStateDuration(int _state);
    int getNextState(int _state);

    // property accessors
    void setState(int _state);
    void setDirection(int _direction);
    void setWayAngle(double _angle);

    Node* getNode();
    int getState();
    int getDirection();
    double getWayAngle();

private:
    void handleEvent(Event *_event);


private:
    Node *p_node;
    Event *m_updateTimer;

    int m_state;
    double m_green_s;
    double m_yellow_s;
    double m_red_s;
    double m_redYellow_s;

    int m_direction;
    double m_wayAngle;
};

}

#endif // TRAFFICSIGNAL_H
