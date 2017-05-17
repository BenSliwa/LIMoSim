#ifndef LIMOSIM_MAPDEFINITIONS_H
#define LIMOSIM_MAPDEFINITIONS_H

namespace LIMOSIM
{
    namespace WAY_TYPE
    {
        enum{
            WAY = 0,
            PRIMARY_HIGHWAY = 1,
            RESIDENTIAL = 2,
        };
    }

    namespace NODE_TYPE
    {
        enum{
            NODE = 0,
            TRAFFIC_SIGNAL = 1,
            TRAFFIC_SIGNAL_CONTROLLER = 2
        };
    }

    namespace WAY_DIRECTION
    {
        enum{
            BIDIRECTIONAL = 0,
            FORWARD = 1,
            BACKWARD = 2
        };
    }

    namespace TURN // order: sharpness, right/left
    {
        enum{
            NONE,
            THROUGH,
            SLIGHT_RIGHT,
            SLIGHT_LEFT,
            RIGHT,
            LEFT,
            SHARP_LEFT,
            SHARP_RIGHT,
            REVERSE,

            SUPERIOR, // primary to residential
            INFERIOR, // residential to primary
            EQUAL // residential to residential
        };
    }

    namespace CONNECTION
    {
        enum
        {
            END2START,
            END2END,
            START2END,
            START2START
        };
    }

}

#endif // LIMOSIM_MAPDEFINITIONS_H
