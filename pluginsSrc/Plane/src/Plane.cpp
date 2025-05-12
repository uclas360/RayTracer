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

#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Plane::Plane() : radius(0) {};
Plane::Plane(Math::Vector3D pos, double radius) : pos(pos), radius(radius) {};

Plane::Plane(const libconfig::Setting &settings) {
  if (!settings.lookupValue("posX", this->pos.x)) {
    throw ParsingException(
        "error parsing plane object, missing \"posX\" field");
  }
  if (!settings.lookupValue("posY", this->pos.y)) {
    throw ParsingException(
        "error parsing plane object, missing \"posY\" field");
  }
  if (!settings.lookupValue("posZ", this->pos.z)) {
    throw ParsingException(
        "error parsing plane object, missing \"posZ\" field");
  }
  if (!settings.lookupValue("orientationX", this->orientation.x)) {
    throw ParsingException(
        "error parsing plane object, missing \"orientationX\" field");
  }
  if (!settings.lookupValue("orientationY", this->orientation.y)) {
    throw ParsingException(
        "error parsing plane object, missing \"orientationY\" field");
  }
  if (!settings.lookupValue("orientationZ", this->orientation.z)) {
    throw ParsingException(
        "error parsing plane object, missing \"orientationZ\" field");
  }
}

HitRecord Plane::hits(const Ray &ray) const {
  double denom = orientation.dot(ray.dir);
  if (std::abs(denom) > EPSILON) {
    Math::Vector3D p0l0 = pos - ray.pos;
    double t = p0l0.dot(orientation) / denom;
    if (t >= 0) {
      HitRecord rec = HitRecord(t, ray, *this, this->orientation);
      return rec;
    }
  }
  return HitRecord();
}

void Plane::move(const Math::Vector3D &offset) { this->pos += offset; }

void Plane::rotate(const Math::Vector3D &angles) { (void)(angles); }

void Plane::scale(size_t scale) { this->radius *= (double)scale; }

void Plane::setPosition(const Math::Vector3D &newPos) { this->pos = newPos; }

std::ostream &operator<<(std::ostream &out, const Plane &plane) {
  return out << "Plane(pos=" << plane.pos << ", radius=" << plane.radius << ")"
             << std::endl;
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::Plane(config);
}
}
