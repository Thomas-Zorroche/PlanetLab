#include "Color.hpp"

namespace PlanetLab
{

Color::Color(float red, float green, float blue)
    : r(clamp(red)), g(clamp(green)), b(clamp(blue)) {}

Color::Color(const glm::vec3& vector)
    : r(clamp(vector.r)), g(clamp(vector.g)), b(clamp(vector.b)) {}

Color::Color(float value)
    : r(clamp(value)), g(clamp(value)), b(clamp(value)) {}

bool operator!=(const Color& lhs, const Color& rhs)
{
    return lhs.r != rhs.r && lhs.g != rhs.g && lhs.b != rhs.b;
}

bool operator==(const Color& lhs, const Color& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

}