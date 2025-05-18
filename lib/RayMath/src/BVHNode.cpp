/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** BVHNode
*/

#include "BVHNode.hpp"

#include <algorithm>
#include <functional>

#include "AABB.hpp"
#include "plugins/AShape.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

bool box_compare(const std::unique_ptr<IShape> &a,
                        const std::unique_ptr<IShape> &b, int axis_index) {
    Interval a_axis_interval = a->boundingBox().axisInterval(axis_index);
    Interval b_axis_interval = b->boundingBox().axisInterval(axis_index);
    return a_axis_interval.min < b_axis_interval.min;
}

BVHNode::BVHNode(std::vector<std::unique_ptr<IShape>> &objects, size_t start,
                 size_t end) {
    this->calculateAABB(objects, start, end);
}

HitRecord BVHNode::hits(const RayTracer::Ray &r, Interval ray_t) const {
    if (bbox.hits(r, ray_t).missed) return RayTracer::HitRecord();

    RayTracer::HitRecord hit_left = this->left->hits(r, ray_t);
    RayTracer::HitRecord hit_right = this->right->hits(
        r, Interval(ray_t.min, hit_left.missed ? ray_t.max : hit_left.t));

    return hit_right.missed ? hit_left : hit_right;
}

void BVHNode::update(void) {
    if (this->parentBvh == nullptr) {
        this->calculateAABB(*this->allObjects, 0, this->allObjects->size());
        if (this->parentObject != nullptr &&
            this->parentObject->getBVH() != nullptr) {
            this->parentObject->getBVH()->update();
        }
    } else {
        this->parentBvh->update();
    }
}

void BVHNode::calculateAABB(std::vector<std::unique_ptr<IShape>> &objects,
                            size_t start, size_t end) {
    this->allObjects = &objects;
    this->bbox.reset();
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
        this->left->setBVH(this);
    } else if (object_span == 2) {
        this->left = objects[start].get();
        this->right = objects[start + 1].get();
        this->left->setBVH(this);
        this->right->setBVH(this);
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        double mid = (double)start + (double)object_span / 2;

        this->bLeft = std::make_unique<BVHNode>(objects, start, mid);
        this->bLeft->parentBvh = this;
        this->left = this->bLeft.get();

        this->bRight = std::make_unique<BVHNode>(objects, mid, end);
        this->bRight->parentBvh = this;
        this->right = this->bRight.get();
    }
    this->bbox =
        RayTracer::AABB(this->left->boundingBox(), this->right->boundingBox());
}

};  // namespace RayTracer
