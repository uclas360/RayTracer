/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** IObject
*/

#pragma once

#include <cstddef>
#include "plugins/IPlugin.hpp"
#include "raytracer/math/Vector.hpp"

class IObject : public IPlugin {
    void move(const Math::Vector3D &offset);
    void rotate(const Math::Vector3D &angles);
    void scale(size_t scale);
    void setPosition(const Math::Vector3D &newPos);
};
