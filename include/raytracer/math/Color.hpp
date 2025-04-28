/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Color
*/

#pragma once

#include <iostream>

namespace Math {

#define COLOR_RANGE(x) (std::max(0, std::min((x), 255)))

class Color {
   public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    Color() = default;
    Color(unsigned char r, unsigned char g, unsigned char b,
          unsigned char a = 255);

    Color operator+(const Color &other) const;
    Color operator-(const Color &other) const;
    private:
    unsigned char getInRangeValue(int) const;

};

std::ostream &operator<<(std::ostream &out, const Color &vec);

}  // namespace Math
