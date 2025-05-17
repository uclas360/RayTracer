/*
** EPITECH PROJECT, 2025
** Box
** File description:
** Box
*/

#ifndef BOX_HPP
#define BOX_HPP

#include <libconfig.h++>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IShape.hpp"
#include "plugins/AShape.hpp"

namespace RayTracer
{
    class Box3D : public AShape
    {
    public:
        Math::Vector3D _tmin;
        Math::Vector3D _tmax;

        Box3D() : _tmin{0, 0, 0}, _tmax{0, 0, 0} {};
        Box3D(const libconfig::Setting &settings);
        Box3D(Math::Vector3D tmin, Math::Vector3D tmax) : _tmin(tmin), _tmax(tmax) {
                                                          };

        HitRecord hits(const Ray &ray, Interval ray_t) const override;
        void move(const Math::Vector3D &offset) override;
        void rotate(const Math::Vector3D &angles) override;
        void scale(size_t scale) override;
        void setPosition(const Math::Vector3D &newPos) override;
        Math::Vector3D getPointColor(const Math::Vector3D &) const override;
    };
};
#endif
