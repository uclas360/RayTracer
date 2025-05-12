/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Cylinder
*/

#include "../include/Cylinder.hpp"

#include <cmath>
#include <libconfig.h++>
#include <ostream>

#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Cylinder::Cylinder(const libconfig::Setting &settings) {
  try {
    libconfig::Setting &pos = settings.lookup("pos");
    if (!Math::lookUpVector(pos, this->pos_)) {
      throw ParsingException(
          "error parsing cylinder object, wrong \"pos\" field");
    }
    if (!settings.lookupValue("radius", this->radius_) ||
        !settings.lookupValue("height", this->height_)) {
      throw ParsingException("error parsing cylinder object, wrong \"radius / height\" field");
    }
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
}

HitRecord Cylinder::hits(const Ray &ray) const {
  Math::Vector3D oc = ray.pos - pos_;
  double a = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
  double b = 2.0 * (oc.x * ray.dir.x + oc.z * ray.dir.z);
  double c = oc.x * oc.x + oc.z * oc.z - radius_ * radius_;

  if (fabs(a) < EPSILON) return hitsCapOnly(ray);
  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0) return hitsCapOnly(ray);
  double sqrtd = sqrt(discriminant);
  double t1 = (-b - sqrtd) / (2 * a);
  double t2 = (-b + sqrtd) / (2 * a);
  double t = t1;
  double y1 = oc.y + t1 * ray.dir.y;

  if (t1 < EPSILON || y1 < 0 || y1 > height_) {
    t = t2;
    double y2 = oc.y + t2 * ray.dir.y;

    if (t2 < EPSILON || y2 < 0 || y2 > height_) return hitsCapOnly(ray);
  }
  Math::Vector3D p = ray.pos + ray.dir * t;
  Math::Vector3D normal(p.x - pos_.x, 0, p.z - pos_.z);
  normal = normal.normalized();
  HitRecord capRecord = hitsCapOnly(ray);

  if (capRecord.t > EPSILON && (capRecord.t < t || t < EPSILON))
    return capRecord;

  return HitRecord(t, ray, *this, normal);
}

HitRecord Cylinder::hitsCapOnly(const Ray &ray) const {
  double t_min = INFINITY;
  Math::Vector3D normal;

  if (fabs(ray.dir.y) > EPSILON) {
    double t = (pos_.y - ray.pos.y) / ray.dir.y;
    if (t > EPSILON) {
      Math::Vector3D p = ray.pos + ray.dir * t;
      double distance = (p - Math::Vector3D(pos_.x, pos_.y, pos_.z)).length();
      if (distance <= radius_) {
        t_min = t;
        normal = Math::Vector3D(0, -1, 0);
      }
    }
  }
  if (fabs(ray.dir.y) > EPSILON) {
    double t = (pos_.y + height_ - ray.pos.y) / ray.dir.y;

    if (t > EPSILON && t < t_min) {
      Math::Vector3D p = ray.pos + ray.dir * t;
      double distance =
          (p - Math::Vector3D(pos_.x, pos_.y + height_, pos_.z)).length();

      if (distance <= radius_) {
        t_min = t;
        normal = Math::Vector3D(0, 1, 0);
      }
    }
  }

  if (t_min < INFINITY) return HitRecord(t_min, ray, *this, normal);

  return HitRecord();
}

void Cylinder::move(const Math::Vector3D &offset) { this->pos_ += offset; }

void Cylinder::rotate(const Math::Vector3D &angles) { (void)(angles); }

void Cylinder::scale(size_t scale) { this->radius_ *= (double)scale; }

void Cylinder::setPosition(const Math::Vector3D &newPos) {
  this->pos_ = newPos;
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::Cylinder(config);
}
}
