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
  double temp;

  try {
    libconfig::Setting &a = settings.lookup("a");
    libconfig::Setting &b = settings.lookup("b");
    libconfig::Setting &c = settings.lookup("c");
    a.lookupValue("x", temp);
    this->a.x = temp;
    a.lookupValue("y", temp);
    this->a.y = temp;
    a.lookupValue("z", temp);
    this->a.z = temp;

    b.lookupValue("x", temp);
    this->b.x = temp;
    b.lookupValue("y", temp);
    this->b.y = temp;
    b.lookupValue("z", temp);
    this->b.z = temp;

    c.lookupValue("x", temp);
    this->c.x = temp;
    c.lookupValue("y", temp);
    this->c.y = temp;
    c.lookupValue("z", temp);
    this->c.z = temp;
  } catch (const libconfig::SettingException) {
    std::cout << "error" << std::endl;
    return;
  }
}

HitRecord Triangle::hits(const Ray &ray) const {
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

  return HitRecord(t, ray, *this, n.normalized());
}
}  // namespace RayTracer

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::Triangle(config);
}
}