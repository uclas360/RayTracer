/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** ILight
*/

#ifndef ILIGHT_HPP
#define ILIGHT_HPP

#include "Raytracer/math/Color.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IObject.hpp"

class ILight : public IObject {
   public:
    virtual Math::Color getColorFrom(const Math::Vector3D &point) const = 0;
};

#endif
