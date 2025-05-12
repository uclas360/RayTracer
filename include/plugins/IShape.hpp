/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** IShape
*/

#ifndef ISHAPE_HPP
#define ISHAPE_HPP

#include <optional>
#include "Raytracer/Ray.hpp"
#include "plugins/IObject.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class HitRecord;

class IShape : public IObject {
 public:
  virtual void setMaterial(std::unique_ptr<Material> &) = 0;
  virtual HitRecord hits(const Ray &ray) const = 0;
};

class HitRecord {
 public:
  Math::Vector3D p;
  Math::Vector3D normal;
  double t = 0;
  bool frontFace = false;
  bool missed = true;
  std::optional<std::reference_wrapper<const IShape>> shapeHit;
  std::optional<std::reference_wrapper<const std::unique_ptr<Material>>> mat;

  HitRecord() = default;
  HitRecord(const double t, const Ray &ray, const IShape &shape,
            const Math::Vector3D &normal);
};

};  // namespace RayTracer

#endif
