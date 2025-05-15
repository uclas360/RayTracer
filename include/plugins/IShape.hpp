/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** IShape
*/

#ifndef ISHAPE_HPP
#define ISHAPE_HPP

#include <optional>

#include "Interval.hpp"
#include "Raytracer/Ray.hpp"
#include "plugins/IObject.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class HitRecord;
class AABB;

class IShape : public IObject {
   public:
    virtual void setMaterial(std::unique_ptr<Material> &) = 0;
    virtual HitRecord hits(const Ray &ray, Interval ray_t) const = 0;
    virtual const AABB &boundingBox() const = 0;
};

};  // namespace RayTracer

#include "AABB.hpp"  // don't move this, trust the process (cross include)

namespace RayTracer {

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
              const Math::Vector3D &normal,
              const std::unique_ptr<Material> &mat);
};

};  // namespace RayTracer

#endif
