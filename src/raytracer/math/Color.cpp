/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Vector
*/

#include "raytracer/math/Color.hpp"
#include <cmath>

Math::Color::Color(unsigned char r, unsigned char g, unsigned char b,
                   unsigned char a)
    : r(r), g(g), b(b), a(a) {
}

Math::Color Math::Color::operator+(const Math::Color &other) const {
    return Math::Color(
        COLOR_RANGE(this->r + other.r),
        COLOR_RANGE(this->g + other.g),
        COLOR_RANGE(this->b + other.b),
        COLOR_RANGE(this->a + other.a)
    );
}

Math::Color Math::Color::operator-(const Math::Color &other) const {
    return Math::Color(
        COLOR_RANGE(this->r - other.r),
        COLOR_RANGE(this->g - other.g),
        COLOR_RANGE(this->b - other.b),
        COLOR_RANGE(this->a - other.a)
    );
}

std::ostream &operator<<(std::ostream &out, const Math::Color &color) {
    out << "Color(r:" << color.r << ",g:" << color.g << ",b:" << color.g << ",a:" << color.a << ")";
    return out;
}
