#ifndef LIMOSIM_DOMDEFINITIONS_H
#define LIMOSIM_DOMDEFINITIONS_H

namespace LIMoSim
{
    namespace DOM
    {
        enum{
            START_TAG,
            END_TAG,
            INLINE_TAG,
            CONTENT
        };
    }
}

namespace LIMoSim
{
    namespace NODE_TYPES
    {
        enum{
            ELEMENT_NODE = 1,
            ATTRIBUTE_NODE = 2,
            TEXT_NODE = 3,
            DOCUMENT_NODE = 9
        };
    }
}

#endif // LIMOSIM_DOMDEFINITIONS_H
