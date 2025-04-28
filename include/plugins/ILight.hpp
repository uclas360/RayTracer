/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** ILight
*/

#pragma once

#include "plugins/IObject.hpp"
#include "raytracer/math/Color.hpp"
#include "raytracer/math/Vector.hpp"

class ILight: public IObject {
    public:
        Math::Color getColorFrom(const Math::Vector3D &point) const;
};
