#include "types.hpp"
#include <cmath>

using lulu::pair;

pair pair::operator+(const pair &other) const
{
    return {x + other.x, y + other.y};
}

pair pair::operator-(const pair &other) const
{
    return {x - other.x, y - other.y};
}

pair pair::operator*(const pair &other) const
{
    return {x * other.x, y * other.y};
}

pair pair::operator/(const pair &other) const
{
    return {x / other.x, y / other.y};
}

pair pair::operator+(const float offset) const
{
    return {x + offset, y + offset};
}

pair pair::operator-(const float offset) const
{
    return {x - offset, y - offset};
}

pair pair::operator*(const float offset) const
{
    return {x * offset, y * offset};
}

pair pair::operator/(const float offset) const
{
    return {x / offset, y / offset};
}

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

std::optional<pair> pair::diagonal() const
{
    if (x == y)
    {
        const auto l = static_cast<float>(x / std::sqrt(2));
        return pair{l, l};
    }
    return std::nullopt;
}
