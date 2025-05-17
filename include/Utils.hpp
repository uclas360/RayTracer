/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** utils
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <limits>
#include <random>
#include <string>

static const std::string HELP_MESSAGE =
    "USAGE: ./raytracer <SCENE_FILE> [-g]\n"
    "\tSCENE_FILE: scene configuration"
    "\t-g graphic: open a sfml window instead of outputing pixels";

#define PI 3.1415926535897932385
#define EPSILON 1e-14

const double DOUBLE_INFINITY = std::numeric_limits<double>::infinity();

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;

    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#endif
