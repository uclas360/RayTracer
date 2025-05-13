/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Sphere
*/

#include "../include/Sphere.hpp"

#include <cmath>
#include <libconfig.h++>
#include <ostream>

#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Sphere::Sphere() : radius(0) {};
Sphere::Sphere(Math::Vector3D pos, double radius) : pos(pos), radius(radius) {};

Sphere::Sphere(const libconfig::Setting &settings) {
  try {
    libconfig::Setting &pos = settings.lookup("pos");
    if (!Math::lookUpVector(pos, this->pos)) {
      throw ParsingException(
          "error parsing sphere object, wrong \"pos\" field");
    }
    if (!settings.lookupValue("radius", this->radius)) {
      throw ParsingException(
          "error parsing sphere object, wrong \"radius\" field");
    }
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
}

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

void Sphere::move(const Math::Vector3D &offset) { this->pos += offset; }

void Sphere::rotate(const Math::Vector3D &angles) { (void)(angles); }

void Sphere::scale(size_t scale) { this->radius *= (double)scale / 100; }

void Sphere::setPosition(const Math::Vector3D &newPos) { this->pos = newPos; }

std::ostream &operator<<(std::ostream &out, const Sphere &sphere) {
  return out << "Sphere(pos=" << sphere.pos << ", radius=" << sphere.radius
             << ")" << std::endl;
}

void Sphere::save(libconfig::Setting &parent) const {
  libconfig::Setting &sphereSettings =
      parent.add(libconfig::Setting::TypeGroup);
  sphereSettings.add("type", libconfig::Setting::TypeString) = "shape";
  sphereSettings.add("name", libconfig::Setting::TypeString) = "sphere";
  libconfig::Setting &data =
      sphereSettings.add("data", libconfig::Setting::TypeGroup);
  libconfig::Setting &posSettings =
      data.add("pos", libconfig::Setting::TypeGroup);
  Math::writeUpVector(posSettings, this->pos);
  data.add("radius", libconfig::Setting::TypeFloat) = radius;
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::Sphere(config);
}
}
