#include "variant.h"
#include <sstream>

namespace LIMOSIM
{

Variant::Variant()
{

}

Variant::Variant(int _data)
{
    std::stringstream stream;
    stream << _data;

    m_data = stream.str();
}

Variant::Variant(double _data)
{
    std::stringstream stream;
    stream << _data;

    m_data = stream.str();
}

Variant::Variant(const std::string &_data)
{
    m_data = _data;
}

int Variant::toInt() const
{
    std::stringstream stream;
    stream << m_data;

    long long result;
    stream >> result;
    return result;
}

double Variant::toDouble() const
{
    std::stringstream stream;
    stream << m_data;

    double result;
    stream >> result;
    return result;
}

std::string Variant::toString() const
{
    return m_data;
}

}

