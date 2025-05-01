/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** IShape
*/

#pragma once

#include "plugins/IObject.hpp"
#include "raytracer/Ray.hpp"

struct HitRecord {
    //
};

class IShape : public IObject {
    public:
    virtual HitRecord hits(const RayTracer::Ray &ray) const = 0;
};
