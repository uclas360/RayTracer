/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Circle
*/

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <iostream>
#include <libconfig.h++>

#include "plugins/AShape.hpp"
#include "plugins/IShape.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"

namespace RayTracer {

class Plane : public AShape {
    public:
    Plane();
    Plane(Math::Vector3D pos, double radius);
    Plane(const libconfig::Setting &);
    HitRecord hits(const Ray &ray, Interval ray_t) const override;
    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void scale(size_t scale) override;
    void setPosition(const Math::Vector3D &newPos) override;

    Math::Vector3D pos;
    Math::Vector3D orientation;
    double radius;
};

std::ostream &operator<<(std::ostream &out, const Plane &ray);

}  // namespace RayTracer



#endif
