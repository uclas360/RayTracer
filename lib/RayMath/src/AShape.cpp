/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Rectangle
*/

#include "plugins/AShape.hpp"

namespace RayTracer {

void AShape::setMaterial(Material &newMaterial) {
    this->material_ = newMaterial;
};

}
