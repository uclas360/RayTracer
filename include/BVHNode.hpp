/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** BVHNode
*/

#ifndef BVH_HPP
#define BVH_HPP

#include <algorithm>
#include <functional>
#include <variant>

#include "AABB.hpp"
#include "FixCrossInclude.hpp"
#include "Raytracer/Ray.hpp"
#include "plugins/AShape.hpp"

class BVHNode : public RayTracer::AShape {
   public:
    BVHNode(std::vector<std::unique_ptr<IShape>> &objects, size_t start,
            size_t end, int depth) {
        this->depth = depth;
        // std::cout << this->depth << std::endl;
        this->bbox =
            RayTracer::AABB(Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY),
                            Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY),
                            Interval(+DOUBLE_INFINITY, -DOUBLE_INFINITY));
        for (size_t object_index = start; object_index < end; object_index++)
            this->bbox = RayTracer::AABB(this->bbox,
                                         objects[object_index]->boundingBox());

        int axis = this->bbox.longestAxis();
        bool (*comparator)(const std::unique_ptr<RayTracer::IShape> &a,
                           const std::unique_ptr<RayTracer::IShape> &b) =
            this->comp[axis];

        size_t object_span = end - start;
        // std::cout << object_span << std::endl;

        if (object_span == 1) {
            this->left = objects[start].get();
            this->right = objects[start].get();
        } else if (object_span == 2) {
            this->left = objects[start].get();
            this->right = objects[start + 1].get();
        } else {
            std::sort(std::begin(objects) + start, std::begin(objects) + end,
            comparator);

            auto mid = start + object_span / 2;
            // printf("[DEBUG 11]\n");
            this->bLeft =
            std::make_unique<BVHNode>(objects, start, mid, depth - 1);
            this->left = this->bLeft.get();
            
            // printf("[DEBUG 12]\n");
            this->bRight =
            std::make_unique<BVHNode>(objects, mid, end, depth - 1);
            this->right = this->bRight.get();
            // printf("[DEBUG 13]\n");
        }

        // printf("[DEBUG 14]\n");

        
        this->bbox = RayTracer::AABB(this->left->boundingBox(),
        this->right->boundingBox());
        // printf("[DEBUG 15]\n");
        // std::cout << this->left << std::endl;
        // std::cout << this->right << std::endl;
        // std::cout << this->bbox << std::endl;
    }

    RayTracer::HitRecord hits(const RayTracer::Ray &r,
                              Interval ray_t) const override {
        // if (this->depth <= 0) {
        //     // printf("[DEBUG]\n");
        //     return this->bbox.hits(r, ray_t);
        // }
        if (bbox.hits(r, ray_t).missed) return RayTracer::HitRecord();

        // return bbox.hits(r, ray_t);
        // printf("[DEBUG]\n");
        // std::cout << this->left << std::endl;
        // std::cout << this->right << std::endl;
        // std::cout << printf("[DEBUG 2]\n");
        RayTracer::HitRecord hit_left = this->left->hits(r, ray_t);
        // std::cout << printf("[DEBUG 3]\n");
        RayTracer::HitRecord hit_right = this->right->hits(
            r, Interval(ray_t.min, hit_left.missed ? ray_t.max : hit_left.t));
        // std::cout << printf("[DEBUG 4]\n");

        return hit_right.missed ? hit_left : hit_right;
    }

    static bool box_compare(const std::unique_ptr<RayTracer::IShape> &a,
                            const std::unique_ptr<RayTracer::IShape> &b,
                            int axis_index) {
        Interval a_axis_interval = a->boundingBox().axisInterval(axis_index);
        Interval b_axis_interval = b->boundingBox().axisInterval(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare(const std::unique_ptr<RayTracer::IShape> &a,
                              const std::unique_ptr<RayTracer::IShape> &b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const std::unique_ptr<RayTracer::IShape> &a,
                              const std::unique_ptr<RayTracer::IShape> &b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const std::unique_ptr<RayTracer::IShape> &a,
                              const std::unique_ptr<RayTracer::IShape> &b) {
        return box_compare(a, b, 2);
    }

    void move(const Math::Vector3D &) {};
    void rotate(const Math::Vector3D &) {};
    void scale(size_t) {};
    void setPosition(const Math::Vector3D &) {};

   private:
    std::vector<std::unique_ptr<IShape>> *objects = nullptr;
    std::unique_ptr<BVHNode> bLeft;
    std::unique_ptr<BVHNode> bRight;
    IShape *left;
    IShape *right;
    bool (*comp[3])(const std::unique_ptr<RayTracer::IShape> &a,
                    const std::unique_ptr<RayTracer::IShape> &b) = {
        box_x_compare, box_y_compare, box_z_compare};
};

#endif
