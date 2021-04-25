#pragma once

#include "glm/glm.hpp"
#include <algorithm>
#include <random>

namespace editor
{

struct Color
{
    float r, g, b;

    /* Constructors */
    Color(float red, float green, float blue)
        : r(clamp(red)), g(clamp(green)), b(clamp(blue)) {}

    Color(const glm::vec3& vector)
        : r(clamp(vector.r)), g(clamp(vector.g)), b(clamp(vector.b)) {}

    Color(float value = 0.8f)
        : r(clamp(value)), g(clamp(value)), b(clamp(value)) {}


    float clamp(float value) { return std::clamp(value, 0.0f, 1.0f); }

    glm::vec3 vector() const { return glm::vec3(r, g, b); }

    bool operator==(const Color& rhs)
    {
        return r == rhs.r && g == rhs.g && b == rhs.b;
    }

    bool operator!=(const Color& rhs)
    {
        return r != rhs.r && g != rhs.g && b != rhs.b;
    }

    friend bool operator!=(const Color& lhs, const Color& rhs);

    friend bool operator==(const Color& lhs, const Color& rhs);
};

}   // ns editor
