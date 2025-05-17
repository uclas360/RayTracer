/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Plane
*/

#include "../include/Plane.hpp"

#include <cmath>
#include <libconfig.h++>
#include <ostream>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Plane::Plane() : radius(0) {};
Plane::Plane(Math::Vector3D pos, double radius) : pos(pos), radius(radius) {};

Plane::Plane(const libconfig::Setting &settings) {
    try {
        libconfig::Setting &pos = settings.lookup("pos");
        libconfig::Setting &normal = settings.lookup("orientation");
        if (!Math::lookUpVector(pos, this->pos)) {
            throw ParsingException(
                "error parsing plane object, wrong \"pos\" field");
        }
        if (!Math::lookUpVector(normal, this->orientation)) {
            throw ParsingException(
                "error parsing plane object, wrong \"orientation\" field");
        }
        std::string texture;
        if (settings.lookupValue("texture", texture)) {
            this->texture_ = texture;
        }
    } catch (const ParsingException &e) {
        throw e;
    } catch (const libconfig::SettingNotFoundException &e) {
        throw ParsingException(e.what());
    }
}

HitRecord Plane::hits(const Ray &ray, Interval ray_t) const {
    double denom = orientation.dot(ray.dir);
    if (std::abs(denom) > EPSILON) {
        Math::Vector3D p0l0 = pos - ray.pos;
        double t = p0l0.dot(orientation) / denom;
        if (t >= 0 && ray_t.contains(t)) {
            return HitRecord(t, ray, *this, this->orientation, this->material_);
        }
    }
    return HitRecord();
}

void Plane::move(const Math::Vector3D &offset) {
    this->pos += offset;
}

void Plane::rotate(const Math::Vector3D &angles) {
    (void)(angles);
}

void Plane::scale(size_t scale) {
    this->radius *= (double)scale;
}

void Plane::setPosition(const Math::Vector3D &newPos) {
    this->pos = newPos;
}

Math::Vector3D Plane::getPointColor(const Math::Vector3D &point) const {
    return this->texture_.getColor(point.x, point.z);
}

std::ostream &operator<<(std::ostream &out, const Plane &plane) {
    return out << "Plane(pos=" << plane.pos << ", radius=" << plane.radius
               << ")" << std::endl;
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new RayTracer::Plane(config);
}
}
