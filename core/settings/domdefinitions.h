#ifndef LIMOSIM_DOMDEFINITIONS_H
#define LIMOSIM_DOMDEFINITIONS_H

namespace LIMOSIM
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

namespace LIMOSIM
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
