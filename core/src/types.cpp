#include "types.hpp"
#include <cmath>

using lulu::pair;

pair &pair::operator+=(const pair &other)
{
    *this = *this + other;
    return *this;
}

pair &pair::operator-=(const pair &other)
{
    *this = *this - other;
    return *this;
}

pair pair::diagonal() const
{
    float l;
    if(x==y) l = x/std::sqrt(2);
    else l = -1;
    return {l, l};
}