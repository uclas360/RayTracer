/*
** EPITECH PROJECT, 2025
** Box
** File description:
** Box
*/

#include <libconfig.h++>
#include <algorithm>

#include "Box3D.hpp"
#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer
{
    Box3D::Box3D(const libconfig::Setting &settings)
    {
        double tminX;
        double tminY;
        double tminZ;
        double tmaxX;
        double tmaxY;
        double tmaxZ;

        if (!settings.lookupValue("tminX", tminX))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tminX\" field");
        }
        if (!settings.lookupValue("tminY", tminY))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tminY\" field");
        }
        if (!settings.lookupValue("tminZ", tminZ))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tminZ\" field");
        }
        if (!settings.lookupValue("tmaxX", tmaxX))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tmaxX\" field");
        }
        if (!settings.lookupValue("tmaxY", tmaxY))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tmaxY\" field");
        }
        if (!settings.lookupValue("tmaxZ", tmaxZ))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tmaxZ\" field");
        }
        this->_tmin = {tminX, tminY, tminZ};
        this->_tmax = {tmaxX, tmaxY, tmaxZ};
    }

    HitRecord Box3D::hits(const Ray &ray) const
    {

        double tmin = ((_tmin.x - ray.pos.x) / ray.dir.x);
        double tmax = ((_tmax.x - ray.pos.x) / ray.dir.x);
        double tymin = ((_tmin.y - ray.pos.y) / ray.dir.y);
        double tymax = ((_tmax.y - ray.pos.y) / ray.dir.y);
        double tzmin = ((_tmin.z - ray.pos.z) / ray.dir.z);
        double tzmax = ((_tmax.z - ray.pos.z) / ray.dir.z);

        if (ray.dir.x == 0)
        {
            if (ray.pos.x < _tmin.x || ray.pos.x > _tmax.x)
                return HitRecord();
        }
        else if (tmin > tmax)
        {
            std::swap(tmin, tmax);
        }
        if (ray.dir.y == 0)
        {
            if (ray.pos.y < _tmin.y || ray.pos.y > _tmax.y)
                return HitRecord();
        }
        else if (tymin > tymax)
        {
            std::swap(tymin, tymax);
        }

        if ((tmin > tymax) || (tymin > tmax))
            return HitRecord();

        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;
        if (ray.dir.z == 0)
        {
            if (ray.pos.z < _tmin.z || ray.pos.z > _tmax.z)
                return HitRecord();
        }
        else if (tzmin > tzmax)
        {
            std::swap(tzmin, tzmax);
        }

        if ((tmin > tzmax) || (tzmin > tmax))
            return HitRecord();

        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        double t_hit = tmin;
        if (tmin < 0)
            t_hit = tmax;
        if (t_hit < 0)
            return HitRecord();

        Math::Vector3D hit_point = ray.at(t_hit);

        Math::Vector3D normal;
        const double epsilon = 1e-6;

        if (std::abs(hit_point.x - _tmin.x) < epsilon)
            normal = {-1, 0, 0};
        else if (std::abs(hit_point.x - _tmax.x) < epsilon)
            normal = {1, 0, 0};
        else if (std::abs(hit_point.y - _tmin.y) < epsilon)
            normal = {0, -1, 0};
        else if (std::abs(hit_point.y - _tmax.y) < epsilon)
            normal = {0, 1, 0};
        else if (std::abs(hit_point.z - _tmin.z) < epsilon)
            normal = {0, 0, -1};
        else if (std::abs(hit_point.z - _tmax.z) < epsilon)
            normal = {0, 0, 1};

        return HitRecord(t_hit, ray, *this, normal);
    }

    void Box3D::move(const Math::Vector3D &pos)
    {
        _tmin += pos;
        _tmax += pos;
    }

    void Box3D::rotate(const Math::Vector3D &angles)
    {
        _tmin.rotateX(angles.x);
        _tmax.rotateX(angles.x);

        _tmin.rotateY(angles.y);
        _tmax.rotateY(angles.y);

        _tmin.rotateZ(angles.z);
        _tmax.rotateZ(angles.z);
    }

    void Box3D::scale(size_t scale)
    {
        Math::Vector3D center = (_tmin + _tmax) / 2.0;

        _tmin = center + (_tmin - center) * (double)scale / 100;
        _tmax = center + (_tmax - center) * (double)scale / 100;
    }

    void Box3D::setPosition(const Math::Vector3D &newPos)
    {
        Math::Vector3D currentCenter = (_tmin + _tmax) / 2.0;
        Math::Vector3D offset = newPos - currentCenter;

        move(offset);
    }
}
