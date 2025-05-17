/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Interval
*/

#include "Interval.hpp"

Interval::Interval() : min(+DOUBLE_INFINITY), max(-DOUBLE_INFINITY) {
}

Interval::Interval(double min, double max) : min(min), max(max) {
}

Interval::Interval(const Interval &a, const Interval &b) {
    min = a.min <= b.min ? a.min : b.min;
    max = a.max >= b.max ? a.max : b.max;
}

double Interval::size() const {
    return max - min;
}

bool Interval::contains(double x) const {
    return min <= x && x <= max;
}

bool Interval::surrounds(double x) const {
    return min < x && x < max;
}

double Interval::mid() const {
    return (max + min) / 2;
}

double Interval::clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

Interval Interval::expand(double delta) const {
    auto padding = delta / 2;
    return Interval(min - padding, max + padding);
}

Interval Interval::operator+(const double displacement) const {
    return Interval(this->min + displacement, this->max + displacement);
}

Interval &Interval::operator+=(const double &displacement) {
    this->min += displacement;
    this->max += displacement;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Interval &ival) {
    std::cout << "Interval["<< ival.min << ":" << ival.max << "]";
    return out;
}
