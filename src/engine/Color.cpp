#include "Color.hpp"

namespace editor
{

bool operator!=(const Color& lhs, const Color& rhs)
{
    return lhs.r != rhs.r && lhs.g != rhs.g && lhs.b != rhs.b;
}

bool operator==(const Color& lhs, const Color& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

}   // ns editor