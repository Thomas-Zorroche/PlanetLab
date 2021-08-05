#pragma once

namespace Ceres
{

struct Color
{
    float r, g, b;

    Color(float red, float green, float blue);
    Color(const glm::vec3& vector);
    Color(float value = 0.8f);

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

}