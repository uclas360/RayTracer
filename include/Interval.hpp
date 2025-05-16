/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Interval
*/

#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "Utils.hpp"
#include <iostream>

class Interval {
   public:
    double min, max;

    Interval();

    Interval(double min, double max);

    Interval(const Interval &a, const Interval &b);

    double size() const;

    bool contains(double x) const;

    bool surrounds(double x) const;

    double mid() const;

    double clamp(double x) const;

    Interval expand(double delta) const;

    Interval operator+(const double displacement) const;

    Interval &operator+=(const double &displacement);

};

std::ostream &operator<<(std::ostream &out, const Interval &ival);

#endif
