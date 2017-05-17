#include "rng.h"

namespace LIMoSim
{

RNG::RNG()
{

}

int RNG::intUniform(int _from, int _to)
{
    int range = _to - _from + 1;
    if(range==0)
    {
        return _from;
    }
    else
    {
        int value = rand() % range + _from;
        return value;
    }
}

}
