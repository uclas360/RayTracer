/*
** EPITECH PROJECT, 2025
** Quad
** File description:
** Quad
*/

#include <libconfig.h++>

#include "Quad.hpp"
#include "RaytracerCore.hpp"
#include <algorithm>

namespace RayTracer
{
    Quad::Quad(const libconfig::Setting &settings)
    {
        double originX;
        double originY;
        double originZ;
        double uX;
        double uY;
        double uZ;
        double vX;
        double vY;
        double vZ;

        if (!settings.lookupValue("originX", originX))
        {
            throw ParsingException(
                "error parsing quad object, missing \"originX\" field");
        }
        if (!settings.lookupValue("originY", originY))
        {
            throw ParsingException(
                "error parsing quad object, missing \"originY\" field");
        }
        if (!settings.lookupValue("originZ", originZ))
        {
            throw ParsingException(
                "error parsing quad object, missing \"originZ\" field");
        }
        if (!settings.lookupValue("uX", uX))
        {
            throw ParsingException(
                "error parsing quad object, missing \"uX\" field");
        }
        if (!settings.lookupValue("uY", uY))
        {
            throw ParsingException(
                "error parsing quad object, missing \"uY\" field");
        }
        if (!settings.lookupValue("uZ", uZ))
        {
            throw ParsingException(
                "error parsing quad object, missing \"uZ\" field");
        }
        if (!settings.lookupValue("vX", vX))
        {
            throw ParsingException(
                "error parsing quad object, missing \"vX\" field");
        }
        if (!settings.lookupValue("vY", vY))
        {
            throw ParsingException(
                "error parsing quad object, missing \"vY\" field");
        }
        if (!settings.lookupValue("vZ", vZ))
        {
            throw ParsingException(
                "error parsing quad object, missing \"vZ\" field");
        }
        this->_origin = {originX, originY, originZ};
        this->_u = {uX, uY, uZ};
        this->_v = {vX, vY, vZ};

        _normal = _u.cross(_v).normalized();

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

    void Quad::scale(size_t scale)
    {
        Math::Vector3D center = _origin + (_u + _v) / 2.0;

        _origin = center + (_origin - center) * (double)scale / 100;
        _u = center + (_u - center) * (double)scale / 100;
        _v = center + (_v - center) * (double)scale / 100;
        *this = Quad(_origin, _u, _v);
    }

    void Quad::move(const Math::Vector3D &pos)
    {
        _origin += pos;

        *this = Quad(_origin, _u, _v);
    }

    void Quad::rotate(const Math::Vector3D &angles)
    {
        Math::Vector3D center = _origin + (_u + _v) / 2.0;
        Math::Vector3D p0 = _origin - center;
        Math::Vector3D p1 = _origin + _u - center;
        Math::Vector3D p2 = _origin + _v - center;

        if (angles.x != 0)
        {
            p0.rotateX(angles.x);
            p1.rotateX(angles.x);
            p2.rotateX(angles.x);
        }

        if (angles.y != 0)
        {
            p0.rotateY(angles.y);
            p1.rotateY(angles.y);
            p2.rotateY(angles.y);
        }

        if (angles.z != 0)
        {
            p0.rotateZ(angles.z);
            p1.rotateZ(angles.z);
            p2.rotateZ(angles.z);
        }

        _origin = center + p0;
        _u = p1 - p0;
        _v = p2 - p0;

        *this = Quad(_origin, _u, _v);
    }

    void Quad::setPosition(const Math::Vector3D &pos)
    {
        Math::Vector3D offset = pos - _origin;

        move(offset);
    }

    HitRecord Quad::hits(const Ray &ray) const
    {
        double denom = _normal.dot(ray.dir);

        if (std::abs(denom) < 1e-6)
            return HitRecord();

        double t = (_origin - ray.pos).dot(_normal) / denom;

        if (t < 0.001)
            return HitRecord();

        Math::Vector3D p = ray.at(t);

        double u_coord = (p - _origin).dot(_u) / _u.dot(_u);
        double v_coord = (p - _origin).dot(_v) / _v.dot(_v);

        if (u_coord < 0 || u_coord > 1 || v_coord < 0 || v_coord > 1)
            return HitRecord();

        return HitRecord(t, ray, *this, _normal);
    }
}

extern "C"
{
    void *entry_point(const libconfig::Setting &config)
    {
        return new RayTracer::Quad(config);
    }
}