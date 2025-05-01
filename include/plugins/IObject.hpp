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
    public:
    virtual void move(const Math::Vector3D &offset) = 0;
    virtual void rotate(const Math::Vector3D &angles) = 0;
    virtual void scale(size_t scale) = 0;
    virtual void setPosition(const Math::Vector3D &newPos) = 0;
};
