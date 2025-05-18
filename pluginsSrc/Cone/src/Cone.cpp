/*
** EPITECH PROJECT, 2025
** Disc
** File description:
** Disc
*/

#include "../include/Cone.hpp"

#include <libconfig.h++>

#include "Interval.hpp"
#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {
Cone::Cone(const libconfig::Setting &settings) {
    try {
        libconfig::Setting &tip = settings.lookup("tipPosition");
        if (!Math::lookUpVector(tip, this->tipPosition_)) {
            throw ParsingException(
                "error parsing cone object, missing \"tipPosition\" field");
        }
    } catch (const libconfig::SettingNotFoundException &) {
        throw ParsingException(
            "error parsing cone object, missing \"tipPosition\" field");
    }
    try {
        libconfig::Setting &tip = settings.lookup("axis");
        if (!Math::lookUpVector(tip, this->axis_)) {
            throw ParsingException(
                "error parsing cone object, missing \"axis\" field");
        }
    } catch (const libconfig::SettingNotFoundException &) {
        throw ParsingException(
            "error parsing cone object, missing \"axis\" field");
    }
    if (!settings.lookupValue("angle", this->angle)) {
        throw ParsingException(
            "error parsing cone object, missing \"angle\" field");
    }
    if (!settings.lookupValue("height", this->h_)) {
        throw ParsingException(
            "error parsing cone object, missing \"height\" field");
    }
    if (!settings.lookupValue("radius", this->radius)) {
        throw ParsingException(
            "error parsing cone object, missing \"radius\" field");
    }
    std::string texture;
    if (settings.lookupValue("texture", texture)) {
      this->texture_ = texture;
    }
    this->bbox = AABB({-10000,-10000,-10000}, {10000,10000,10000});
}

HitRecord Cone::hits(const Ray &ray, Interval) const {
    double A = ray.pos.x - this->tipPosition_.x;
    double B = ray.pos.z - this->tipPosition_.z;
    double C = h_ - ray.pos.y + this->tipPosition_.y;
   // double tan = (this->radius / h_) * (radius / h_);
    double a = (ray.dir.x * ray.dir.x) + (ray.dir.z * ray.dir.z) -
               ((cos(ray.dir.y) * cos(ray.dir.y)));
    double b = (2 * A * cos(ray.dir.x)) + (2 * B * cos(ray.dir.z)) +
               (2 * C * cos(ray.dir.y));
    double c = (A * A) + (B * B) - ((C * C));
    double delta = b * b - 4 * (a * c);

    if (fabs(delta) < EPSILON) {
        return HitRecord();
    }
    double t1 = (-b - sqrt(delta)) / (2 * a);
    double t2 = (-b + sqrt(delta)) / (2 * a);
    double t;

    if (t1 > t2) {
        t = t2;
    } else {
        t = t1;
    }
    double r = ray.pos.y + t * ray.dir.y;
    Math::Vector3D p = ray.at(t);
    double r2 =
        sqrt((p.x - this->tipPosition_.x) * (p.x - this->tipPosition_.x) +
             (p.z - this->tipPosition_.z) * (p.z - this->tipPosition_.z));
    Math::Vector3D normal = {p.x - this->tipPosition_.x, r2 * (radius / h_),
                             p.z - this->tipPosition_.z};
    normal.normalized();
    if ((r > this->tipPosition_.y) && r < this->tipPosition_.y + h_) {
        return HitRecord(t, ray, *this, normal, this->material_);
    } else {
        return HitRecord();
    }
}

Math::Vector3D Cone::getPointColor(const Math::Vector3D &point) const {
    double theta = std::atan2(point.x, point.z);
    double raw_u = theta / (2 * M_PI);
    double u = 1 - (raw_u + 0.5);

    double origin = this->tipPosition_.x + this->h_;
    double v = (point.y - origin) / this->h_;

    return this->texture_.getColor(u, v);
}

void Cone::save(libconfig::Setting &parent) const {
  libconfig::Setting &coneSettings =
      parent.add(libconfig::Setting::TypeGroup);
  coneSettings.add("type", libconfig::Setting::TypeString) = "shape";
  coneSettings.add("name", libconfig::Setting::TypeString) = "cone";
  libconfig::Setting &data =
      coneSettings.add("data", libconfig::Setting::TypeGroup);
  libconfig::Setting &posSettings =
      data.add("tipPosition", libconfig::Setting::TypeGroup);
  Math::writeUpVector(posSettings, this->tipPosition_);
  libconfig::Setting &uSettings =
      data.add("axis", libconfig::Setting::TypeGroup);
  Math::writeUpVector(uSettings, this->axis_);
  data.add("angle", libconfig::Setting::TypeFloat) = this->angle;
  data.add("height", libconfig::Setting::TypeFloat) = this->h_;
  data.add("radius", libconfig::Setting::TypeFloat) = this->radius;
  if (this->texture_.hasValue()) {
      data.add("texture", libconfig::Setting::TypeString) = this->texture_.getName();
  }
  this->material_->save(coneSettings);
}

}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new RayTracer::Cone(config);
}
}
