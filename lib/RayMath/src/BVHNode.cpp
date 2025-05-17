/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** BVHNode
*/

#include "BVHNode.hpp"

#include <algorithm>
#include <functional>

#include "plugins/AShape.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

BVHNode::BVHNode(std::vector<std::unique_ptr<IShape>> &objects, size_t start,
                 size_t end)
    : AShape(RayTracer::AABB(Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY),
                             Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY),
                             Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY))) {
    for (size_t object_index = start; object_index < end; object_index++)
        this->bbox =
            RayTracer::AABB(this->bbox, objects[object_index]->boundingBox());

    int axis = this->bbox.longestAxis();
    std::function<bool(const std::unique_ptr<RayTracer::IShape> &a,
                       const std::unique_ptr<RayTracer::IShape> &b)>
        comparator = this->comp[axis];

    size_t object_span = end - start;

    if (object_span == 1) {
        this->left = objects[start].get();
        this->right = objects[start].get();
    } else if (object_span == 2) {
        this->left = objects[start].get();
        this->right = objects[start + 1].get();
    } else {
        std::sort(objects.begin() + start, objects.begin() + end,
                  comparator);

        double mid = (double)start + (double)object_span / 2;

        this->bLeft = std::make_unique<BVHNode>(objects, start, mid);
        this->left = this->bLeft.get();

        this->bRight = std::make_unique<BVHNode>(objects, mid, end);
        this->right = this->bRight.get();
    }

    this->bbox =
        RayTracer::AABB(this->left->boundingBox(), this->right->boundingBox());
}

HitRecord BVHNode::hits(const RayTracer::Ray &r, Interval ray_t) const {
    if (bbox.hits(r, ray_t).missed) return RayTracer::HitRecord();

    RayTracer::HitRecord hit_left = this->left->hits(r, ray_t);
    RayTracer::HitRecord hit_right = this->right->hits(
        r, Interval(ray_t.min, hit_left.missed ? ray_t.max : hit_left.t));

    return hit_right.missed ? hit_left : hit_right;
}

};  // namespace RayTracer
