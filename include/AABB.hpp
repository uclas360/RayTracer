/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** AABB
*/

#ifndef AABB_HPP
#define AABB_HPP

#include "FixCrossInclude.hpp"
#include "Interval.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IShape.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class LambertianDebug : public Material {
   public:
    LambertianDebug(const Math::Vector3D &albedo);

    bool scatter(const Ray &r_in, const HitRecord &rec,
                 Math::Vector3D &attenuation, Ray &scattered) const override;
    virtual std::unique_ptr<Material> duplicate(void) override;

   private:
    Math::Vector3D albedo;
};

class AABB : public IShape {
   public:
    Interval x, y, z;

    std::unique_ptr<Material> material_;

    AABB();
    AABB(const Interval &x, const Interval &y, const Interval &z);
    AABB(const Math::Vector3D &a, const Math::Vector3D &b);
    AABB(const AABB &box0, const AABB &box1);

    const Interval &axisInterval(int n) const;
    bool trueHit(const Ray &r, Interval ray_t) const;

    HitRecord hits(const Ray &r, Interval ray_t) const override;

    int longestAxis() const;
    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void scale(size_t scale) override;
    void setPosition(const Math::Vector3D &newPos) override;
    const AABB &boundingBox() const override;
    void setMaterial(std::unique_ptr<Material> &newMaterial) override;

   private:
    void padToMinimums();
};

// AABB operator+(const AABB &bbox, const Math::Vector3D &offset) {
//     return AABB(bbox.x + offset.x, bbox.y + offset.y, bbox.z + offset.z);
// }

// AABB operator+(const Math::Vector3D &offset, const AABB &bbox) {
//     return bbox + offset;
// }
}  // namespace RayTracer

#endif
