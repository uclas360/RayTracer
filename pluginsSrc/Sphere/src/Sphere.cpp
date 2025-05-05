/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Sphere
*/

#include "../include/Sphere.hpp"

#include <cmath>
#include <ostream>
#include <libconfig.h++>

#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Sphere::Sphere() : radius(0) {};
Sphere::Sphere(Math::Vector3D pos, double radius) : pos(pos), radius(radius) {};

Sphere::Sphere(const libconfig::Setting &settings) {
    double posX;
    double posY;
    double posZ;
    double radius;

    if (!settings.lookupValue("posX", posX)) {
        throw ParsingException("error parsing sphere object, missing \"posX\" field");
    }
    if (!settings.lookupValue("posY", posY)) {
        throw ParsingException("error parsing sphere object, missing \"posY\" field");
    }
    if (!settings.lookupValue("posZ", posZ)) {
        throw ParsingException("error parsing sphere object, missing \"posZ\" field");
    }
    if (!settings.lookupValue("radius", radius)) {
        throw ParsingException("error parsing sphere object, missing \"radius\" field");
    }
    this->radius = radius;
    this->pos = {posX, posY, posZ};
}

// bool solveQuadratic(const double a, const double b, const double c, double &x0,
//                     double &x1) {
//     double discr = b * b - 4.0 * a * c;
//     if (discr < 0.0) return false;

//     if (discr == 0.0) {
//         x0 = x1 = -0.5 * b / a;
//     } else {
//         double sqrtDiscr = std::sqrt(discr);
//         double q = (b > 0.0) ? -0.5 * (b + sqrtDiscr) : -0.5 * (b - sqrtDiscr);
//         x0 = q / a;
//         x1 = c / q;
//         if (x0 > x1) std::swap(x0, x1);
//     }
//     return true;
// }

// HitRecord Sphere::hit(const Ray &ray) const {
//     Math::Vector3D L = ray.pos - this->pos;
//     double a = ray.dir.dot(ray.dir);
//     double b = 2.0 * ray.dir.dot(L);
//     double c = L.dot(L) - radius * radius;
//     double t0;
//     double t1;

//     if (!solveQuadratic(a, b, c, t0, t1)) return HitRecord();

//     if (t0 < 0.0) {
//         t0 = t1;
//         if (t0 < 0.0) return HitRecord();
//     }

//     return HitRecord(t0, ray, *this, (ray.at(t0) - this->pos) / this->radius);
// }

HitRecord Sphere::hits(const Ray &ray) const {
    Math::Vector3D L = ray.pos - this->pos;
    double a = ray.dir.dot(ray.dir);
    double b = ray.dir.dot(L);
    double c = L.dot(L) - radius * radius;
    double discr = b * b - a * c;

    if (discr < EPSILON) return HitRecord();

    double sqrtDiscr = std::sqrt(discr);
    double t0 = (-b - sqrtDiscr) / a;
    double t1 = (-b + sqrtDiscr) / a;
    double t = (t0 > 0.0) ? t0 : t1;

    if (t < 0.0) return HitRecord();

    return HitRecord(t, ray, *this, (ray.at(t) - this->pos) / this->radius);
}

void Sphere::move(const Math::Vector3D &offset) {
    this->pos += offset;
}

void Sphere::rotate(const Math::Vector3D &angles) {
    (void)(angles);
}

void Sphere::scale(size_t scale) {
    this->radius *= (double)scale;
}

void Sphere::setPosition(const Math::Vector3D &newPos) {
    this->pos = newPos;
}

std::ostream &operator<<(std::ostream &out, const Sphere &sphere) {
    return out << "Sphere(pos=" << sphere.pos << ", radius=" << sphere.radius << ")"
        << std::endl;
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new RayTracer::Sphere(config);
}
}
