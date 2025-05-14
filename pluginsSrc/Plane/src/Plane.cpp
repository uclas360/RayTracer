/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Plane
*/

#include "Plane.hpp"

#include <cmath>
#include <libconfig.h++>
#include <ostream>

#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Plane::Plane() : radius_(0) {};
Plane::Plane(Math::Vector3D pos, double radius) : pos_(pos), radius_(radius) {};

Plane::Plane(const libconfig::Setting &settings) {
  try {
    libconfig::Setting &pos = settings.lookup("pos");
    libconfig::Setting &normal = settings.lookup("orientation");
    if (!Math::lookUpVector(pos, this->pos_)) {
      throw ParsingException(
          "error parsing plane object, wrong \"pos\" field");
    }
    if (!Math::lookUpVector(normal, this->orientation_)) {
      throw ParsingException(
          "error parsing plane object, wrong \"orientation\" field");
    }
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
}

HitRecord Plane::hits(const Ray &ray) const {
  double denom = orientation_.dot(ray.dir);
  if (std::abs(denom) > EPSILON) {
    Math::Vector3D p0l0 = pos_ - ray.pos;
    double t = p0l0.dot(orientation_) / denom;
    if (t >= 0) {
      HitRecord rec = HitRecord(t, ray, *this, this->orientation_);
      return rec;
    }
  }
  return HitRecord();
}

void Plane::move(const Math::Vector3D &offset) { this->pos_ += offset; }

void Plane::rotate(const Math::Vector3D &angles) { (void)(angles); }

void Plane::scale(size_t scale) { this->radius_ *= (double)scale; }

void Plane::setPosition(const Math::Vector3D &newPos) { this->pos_ = newPos; }

std::ostream &operator<<(std::ostream &out, const Plane &plane) {
  return out << "Plane(pos=" << plane.pos_ << ", radius=" << plane.radius_ << ")"
             << std::endl;
}

void Plane::save(libconfig::Setting &parent) const {
    libconfig::Setting &sphereSettings =
        parent.add(libconfig::Setting::TypeGroup);
    sphereSettings.add("type", libconfig::Setting::TypeString) = "shape";
    sphereSettings.add("name", libconfig::Setting::TypeString) = "cylinder";
    libconfig::Setting &data =
        sphereSettings.add("data", libconfig::Setting::TypeGroup);
    libconfig::Setting &posSettings =
        data.add("pos", libconfig::Setting::TypeGroup);
    Math::writeUpVector(posSettings, this->pos_);
    libconfig::Setting &orientationSettings =
        data.add("orientation", libconfig::Setting::TypeGroup);
    Math::writeUpVector(orientationSettings, this->orientation_);
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::Plane(config);
}
}
