/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** Ray
*/

#pragma once

#include "raytracer/math/Vector.hpp"
#include <libconfig.h++>

namespace RayTracer {

class Camera {
   public:
    Camera() = default;
    Camera(libconfig::Setting &);
    Camera(Camera &&);
    Math::Vector3D pos;
};

}  // namespace RayTracer
