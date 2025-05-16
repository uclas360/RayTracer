/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** BVHNode
*/

#ifndef BVH_HPP
#define BVH_HPP

#include <array>
#include <functional>
#include <vector>
#include "AABB.hpp"
#include "CustomException.hpp"
#include "FixCrossInclude.hpp"
#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/AShape.hpp"

namespace RayTracer {

class BVHException: public CustomException {
    public:
    BVHException(std::string message): CustomException(message) {};
};

static bool box_compare(const std::unique_ptr<IShape> &a,
                            const std::unique_ptr<IShape> &b,
                            int axis_index) {
    Interval a_axis_interval = a->boundingBox().axisInterval(axis_index);
    Interval b_axis_interval = b->boundingBox().axisInterval(axis_index);
    return a_axis_interval.min < b_axis_interval.min;
}

class BVHNode : public AShape {
   public:
    BVHNode(std::vector<std::unique_ptr<IShape>> &objects, size_t start, size_t end);

    RayTracer::HitRecord hits(const RayTracer::Ray &r,
                              Interval ray_t) const override;

    void move(const Math::Vector3D &) override {};
    void rotate(const Math::Vector3D &) override {};
    void scale(size_t) override {};
    void setPosition(const Math::Vector3D &) override {};
    Math::Vector3D getPointColor(const Math::Vector3D &) const override {return {1, 1, 1};};

   private:
    std::unique_ptr<BVHNode> bLeft;
    std::unique_ptr<BVHNode> bRight;
    IShape *left;
    IShape *right;
    std::array<std::function<bool(const std::unique_ptr<IShape> &a,
                    const std::unique_ptr<IShape> &b)>, 3> comp{
        [](const std::unique_ptr<IShape> &a,
                        const std::unique_ptr<IShape> &b) {return box_compare(a, b, 0);},
        [](const std::unique_ptr<IShape> &a,
                        const std::unique_ptr<IShape> &b) {return box_compare(a, b, 1);},
        [](const std::unique_ptr<IShape> &a,
                        const std::unique_ptr<IShape> &b) {return box_compare(a, b, 2);},
    };
};
};

#endif
