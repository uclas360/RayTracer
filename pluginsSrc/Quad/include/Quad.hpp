/*
** EPITECH PROJECT, 2025
** Quad
** File description:
** Quad
*/

#ifndef QUAD_HPP
#define QUAD_HPP

#include <libconfig.h++>
#include <algorithm>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IShape.hpp"
#include "Box3D.hpp"

namespace RayTracer
{
    class Quad : public AShape
    {
    public:
        Math::Vector3D _origin;
        Math::Vector3D _u;
        Math::Vector3D _v;
        Math::Vector3D _normal;
        Box3D _box;
        std::unique_ptr<Material> material_;

        Quad() {};
        ~Quad() = default;
        Quad(const libconfig::Setting &settings);
        Quad(Math::Vector3D q, Math::Vector3D u, Math::Vector3D v) : _origin(q), _u(u), _v(v)
        {
            _normal = u.cross(v).normalized();

            Math::Vector3D origin = _origin;
            Math::Vector3D p1 = _origin + _u;
            Math::Vector3D p2 = _origin + _v;
            Math::Vector3D p3 = _origin + _u + _v;

            double min_x = std::min({origin.x, p1.x, p2.x, p3.x});
            double min_y = std::min({origin.y, p1.y, p2.y, p3.y});
            double min_z = std::min({origin.z, p1.z, p2.z, p3.z});
            double max_x = std::max({origin.x, p1.x, p2.x, p3.x});
            double max_y = std::max({origin.y, p1.y, p2.y, p3.y});
            double max_z = std::max({origin.z, p1.z, p2.z, p3.z});
            _box = Box3D({min_x, min_y, min_z}, {max_x, max_y, max_z});
        }

        void scale(size_t scale) override;
        void move(const Math::Vector3D &offset) override;
        void rotate(const Math::Vector3D &angles) override;
        void setPosition(const Math::Vector3D &newPos) override;
        HitRecord hits(const Ray &ray, Interval ray_t) const override;
        Math::Vector3D getPointColor(const Math::Vector3D &point) const override;
    };
}

#endif
