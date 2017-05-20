#ifndef VARIANT_H
#define VARIANT_H

#include <string>

namespace LIMoSim
{

class Variant
{
public:
    Variant();
    Variant(int _data);
    Variant(double _data);
    Variant(const std::string &_data);

    int toInt() const;
    double toDouble() const;
    std::string toString() const;

private:
    std::string m_data;

};

}

#endif // VARIANT_H
