/*
** EPITECH PROJECT, 2025
** Disc
** File description:
** Disc
*/

#include "../include/Cone.hpp"

#include <algorithm>
#include <libconfig.h++>

#include "Interval.hpp"
#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer
{
    Cone::Cone(const libconfig::Setting &settings)
    {
        try
        {
            libconfig::Setting &tip = settings.lookup("tipPosition");
            if (!Math::lookUpVector(tip, this->tipPosition_))
            {
                throw ParsingException(
                    "error parsing cone object, missing \"tipPosition\" field");
            }
        }
        catch (const libconfig::SettingNotFoundException &)
        {
            throw ParsingException(
                "error parsing cone object, missing \"tipPosition\" field");
        }
        try
        {
            libconfig::Setting &tip = settings.lookup("axis");
            if (!Math::lookUpVector(tip, this->axis_))
            {
                throw ParsingException(
                    "error parsing cone object, missing \"axis\" field");
            }
        }
        catch (const libconfig::SettingNotFoundException &)
        {
            throw ParsingException(
                "error parsing cone object, missing \"axis\" field");
        }
        if (!settings.lookupValue("angle", this->angle))
        {
            throw ParsingException(
                "error parsing cone object, missing \"angle\" field");
        }
        if (!settings.lookupValue("height", this->h_))
        {
            throw ParsingException(
                "error parsing cone object, missing \"height\" field");
        }
        if (!settings.lookupValue("radius", this->radius))
        {
            throw ParsingException(
                "error parsing cone object, missing \"radius\" field");
        }
        std::string texture;
        if (settings.lookupValue("texture", texture))
        {
            this->texture_ = texture;
        }
        this->bbox = AABB({-10000, -10000, -10000}, {10000, 10000, 10000});
    }

    HitRecord Cone::hits(const Ray &ray, Interval) const
    {
        Math::Vector3D oc = ray.pos - tipPosition_;

        double k = (radius / h_) * (radius / h_);

        double a = (ray.dir.x * ray.dir.x) + (ray.dir.z * ray.dir.z) - k * (ray.dir.y * ray.dir.y);
        double b = 2 * (oc.x * ray.dir.x + oc.z * ray.dir.z - k * oc.y * ray.dir.y);
        double c = (oc.x * oc.x) + (oc.z * oc.z) - k * (oc.y * oc.y);

        double discriminant = b * b - 4 * a * c;

        if (discriminant < EPSILON)
            return HitRecord();

        double sqrt_disc = std::sqrt(discriminant);
        double t1 = (-b - sqrt_disc) / (2 * a);
        double t2 = (-b + sqrt_disc) / (2 * a);

        double t = t1;
        if (t1 < EPSILON)
            t = t2;
        if (t < EPSILON)
            return HitRecord();

        Math::Vector3D p = ray.at(t);
        double y = p.y - tipPosition_.y;

        if (y < 0 || y > h_)
            return HitRecord();

        Math::Vector3D d = p - tipPosition_;
        double r = std::sqrt(d.x * d.x + d.z * d.z);
        Math::Vector3D normal(d.x, r * (radius / h_), d.z);
        normal = normal.normalized();

        return HitRecord(t, ray, *this, normal, this->material_);
    }
    Math::Vector3D Cone::getPointColor(const Math::Vector3D &point) const
    {
        double theta = std::atan2(point.x, point.z);
        double raw_u = theta / (2 * M_PI);
        double u = 1 - (raw_u + 0.5);

        double origin = this->tipPosition_.y + this->h_;
        double v = (point.y - origin) / this->h_;

        return this->texture_.getColor(u, v);
    }
}

extern "C"
{
    void *entry_point(const libconfig::Setting &config)
    {
        return new RayTracer::Cone(config);
    }
}
