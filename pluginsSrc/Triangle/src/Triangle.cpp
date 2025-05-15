/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle
*/

#include "Triangle.hpp"

#include <libconfig.h++>

#include "RaytracerCore.hpp"

namespace RayTracer {

Triangle::Triangle(const libconfig::Setting &settings) {
    try {
        libconfig::Setting &a = settings.lookup("a");
        libconfig::Setting &b = settings.lookup("b");
        libconfig::Setting &c = settings.lookup("c");
        if (!Math::lookUpVector(a, this->a))
            throw ParsingException(
                "error parsing triangle object, wrong \"a\" field");
        if (!Math::lookUpVector(b, this->b))
            throw ParsingException(
                "error parsing triangle object, wrong \"b\" field");
        if (!Math::lookUpVector(c, this->c))
            throw ParsingException(
                "error parsing triangle object, wrong \"c\" field");
    } catch (const libconfig::SettingNotFoundException &e) {
        throw ParsingException(e.what());
    } catch (const ParsingException &e) {
        throw ParsingException(e.what());
    }
}

void Triangle::scale(size_t scale) {
    Math::Vector3D center = (a + b + c) / 3.0;

    a = center + (a - center) * (double)scale / 100.;
    b = center + (b - center) * (double)scale / 100.;
    c = center + (c - center) * (double)scale / 100.;
}

void Triangle::setPosition(const Math::Vector3D &) {
}

void Triangle::move(const Math::Vector3D &pos) {
    a += pos;
    b += pos;
    c += pos;
}

void Triangle::rotate(const Math::Vector3D &angles) {
    // Math::Vector3D center = (a + b + c) / 3;
    // Math::Vector3D toOrigin = -center;
    // a += toOrigin;
    // b += toOrigin;
    // c += toOrigin;
    if (angles.y) {
        a.rotateY(angles.y);
        b.rotateY(angles.y);
        c.rotateY(angles.y);
    }
    // a -= toOrigin;
    // b -= toOrigin;
    // c -= toOrigin;
}

HitRecord Triangle::hits(const Ray &ray, Interval ray_t) const {
    Math::Vector3D ab = b - a;
    Math::Vector3D ac = c - a;
    Math::Vector3D n = ab.cross(ac);

    double dot = n.dot(ray.dir);
    if (fabs(dot) < EPSILON) return HitRecord();
    double d = -n.dot(a);
    double t = -(n.dot(ray.pos) + d) / dot;

    if (t < 0) return HitRecord();
    Math::Vector3D p = ray.pos + ray.dir * t;
    Math::Vector3D ne;

    Math::Vector3D ap = p - a;
    ne = ab.cross(ap);
    if (n.dot(ne) < 0) return HitRecord();

    Math::Vector3D bc = c - b;
    Math::Vector3D bp = p - b;
    ne = bc.cross(bp);
    if (n.dot(ne) < 0) return HitRecord();

    Math::Vector3D ca = a - c;
    Math::Vector3D cp = p - c;
    ne = ca.cross(cp);
    if (n.dot(ne) < 0) return HitRecord();

    if (!ray_t.contains(t)) return HitRecord();

    return HitRecord(t, ray, *this, n.normalized(), this->material_);
}
}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new RayTracer::Triangle(config);
}

void *value_entry_point(Math::Vector3D a, Math::Vector3D b, Math::Vector3D c) {
    return new RayTracer::Triangle(a, b, c);
}
}
