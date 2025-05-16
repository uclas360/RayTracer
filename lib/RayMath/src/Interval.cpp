/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Interval
*/

#include "Interval.hpp"

std::ostream &operator<<(std::ostream &out, const Interval &ival) {
    std::cout << "Interval["<< ival.min << ":" << ival.max << "]";
    return out;
}
