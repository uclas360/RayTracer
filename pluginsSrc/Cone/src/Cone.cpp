/*
** EPITECH PROJECT, 2025
** Disc
** File description:
** Disc
*/

#include <libconfig.h++>

#include "Cone.hpp"
#include "RaytracerCore.hpp"
#include <algorithm>

namespace RayTracer
{
    Cone::Cone(const libconfig::Setting &settings)
    {
        double halfAngle;
        double height;
        double tipPositionX;
        double tipPositionY;
        double tipPositionZ;
        double axisX;
        double axisY;
        double axisZ;

        if (!settings.lookupValue("tipPositionX", tipPositionX))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tipPositionX\" field");
        }
        if (!settings.lookupValue("tipPositionY", tipPositionY))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tipPositionY\" field");
        }
        if (!settings.lookupValue("tipPositionZ", tipPositionZ))
        {
            throw ParsingException(
                "error parsing quad object, missing \"tipPositionZ\" field");
        }
        if (!settings.lookupValue("axisX", axisX))
        {
            throw ParsingException(
                "error parsing quad object, missing \"axisX\" field");
        }
        if (!settings.lookupValue("axisY", axisY))
        {
            throw ParsingException(
                "error parsing quad object, missing \"axisY\" field");
        }
        if (!settings.lookupValue("axisZ", axisZ))
        {
            throw ParsingException(
                "error parsing quad object, missing \"axisZ\" field");
        }
        if (!settings.lookupValue("halfAngle", halfAngle))
        {
            throw ParsingException(
                "error parsing quad object, missing \"halfAngle\" field");
        }
        if (!settings.lookupValue("height", height))
        {
            throw ParsingException(
                "error parsing quad object, missing \"height\" field");
        }
        if (!settings.lookupValue("radius", radius))
        {
            throw ParsingException(
                "error parsing quad object, missing \"height\" field");
        }
        this->axis_ = {axisX, axisY, axisZ};
        this->angle = halfAngle;
        this->h_ = height;
        this->radius = radius;
        this->tipPosition_ = {tipPositionX, tipPositionY, tipPositionZ};
    }

    HitRecord Cone::hits(const Ray &ray) const
    {

        double A = ray.pos.x - this->tipPosition_.x;
        double B = ray.pos.z - this->tipPosition_.z;
        double C = h_ - ray.pos.y + this->tipPosition_.y;
        double tan = (this->radius / h_) * (radius / h_);
        double a = (ray.dir.x * ray.dir.x) + (ray.dir.z * ray.dir.z) - ((cos(ray.dir.y) * cos(ray.dir.y)));
        double b = (2 * A * cos(ray.dir.x)) + (2 * B * cos(ray.dir.z)) + (2 * C * cos(ray.dir.y));
        double c = (A * A) + (B * B) - ((C * C));
        double delta = b * b - 4 * (a * c);

        if (fabs(delta) < EPSILON)
        {
            return HitRecord();
        }
        double t1 = (-b - sqrt(delta)) / (2 * a);
        double t2 = (-b + sqrt(delta)) / (2 * a);
        double t;

        if (t1 > t2)
        {
            t = t2;
        }
        else
        {
            t = t1;
        }
        double r = ray.pos.y + t * ray.dir.y;
        Math::Vector3D p = ray.at(t);
        double r2 = sqrt((p.x - this->tipPosition_.x) * (p.x - this->tipPosition_.x) + (p.z - this->tipPosition_.z) * (p.z - this->tipPosition_.z));
        Math::Vector3D normal = {p.x - this->tipPosition_.x, r2 * (radius / h_), p.z - this->tipPosition_.z};
        normal.normalized();
        if ((r > this->tipPosition_.y) && r < this->tipPosition_.y + h_)
        {
            return HitRecord(t, ray, *this, normal);
        }
        else
        {
            return HitRecord();
        }
    }
}

extern "C"
{
    void *entry_point(const libconfig::Setting &config)
    {
        return new RayTracer::Cone(config);
    }
}