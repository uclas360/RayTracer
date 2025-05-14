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

#include "plugins/IShape.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"

namespace RayTracer {

class Plane : public IShape {
    public:
    Plane();
    Plane(Math::Vector3D pos, double radius);
    Plane(const libconfig::Setting &);
    HitRecord hits(const Ray &ray) const override;
    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void scale(size_t scale) override;
    void setPosition(const Math::Vector3D &newPos) override;
    void save(libconfig::Setting &parent) const override;

    Math::Vector3D pos_;
    Math::Vector3D orientation_;
    double radius_;
};

std::ostream &operator<<(std::ostream &out, const Plane &ray);

}  // namespace RayTracer



#endif
