#ifndef RNG_H
#define RNG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace LIMOSIM
{

class RNG
{
public:
    RNG();

    static int intUniform(int _from, int _to);
};

}

#endif // RNG_H
