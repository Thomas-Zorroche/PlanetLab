#include "Color.hpp"

bool operator!=(const Color& lhs, const Color& rhs)
{
    return lhs.r != rhs.r && lhs.g != rhs.g && lhs.b != rhs.b;
}

bool operator==(const Color& lhs, const Color& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

