/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** AObject
*/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IPlugin.hpp"

namespace RayTracer  {

class HitRecord;  // this fix cross include, trust the process

class Material: public IPlugin {
   public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                         Math::Vector3D &attenuation, Ray &scattered) const = 0;
};

};

#endif
