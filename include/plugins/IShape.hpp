/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** IShape
*/

#ifndef ISHAPE_HPP
#define ISHAPE_HPP

#include "Raytracer/Ray.hpp"
#include "plugins/IObject.hpp"

struct HitRecord {
    //
};

class IShape : public IObject {
   public:
    virtual HitRecord hits(const RayTracer::Ray &ray) const = 0;
};

#endif
