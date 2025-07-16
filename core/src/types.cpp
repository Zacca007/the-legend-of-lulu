#include "types.hpp"

#include <cmath>
namespace lulu
{
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

// Arithmetic operations with scalar values
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

// Compound assignment operations
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
        // Calculate diagonal component using Pythagorean theorem
        const auto l = static_cast<float>(x / std::sqrt(2));
        return pair{l, l};
    }
    return std::nullopt;
}
} // namespace lulu