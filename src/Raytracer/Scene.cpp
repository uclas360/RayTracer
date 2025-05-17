/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Scene
*/

#include "Raytracer/Scene.hpp"

#include <cstdio>
#include <memory>

#include "BVHNode.hpp"
#include "Utils.hpp"
#include "plugins/ILight.hpp"

namespace RayTracer {

Scene::Scene(std::unique_ptr<IShape> shape) {
    this->addShape(std::move(shape));
}

HitRecord RayTracer::Scene::hits(const Ray &ray, Interval ray_t) const {
    if (!this->bvh) {
        throw BVHException("scene bvh null");
    }
    return this->bvh->hits(ray, ray_t);
}

void Scene::addShape(std::unique_ptr<IShape> shape) {
    this->bbox = AABB(bbox, shape->boundingBox());
    this->shapes_.emplace_back(std::move(shape));
}

void Scene::addLight(std::unique_ptr<ILight> shape) {
    this->lights_.emplace_back(std::move(shape));
}

void Scene::move(const Math::Vector3D &offset) {
    for (auto &it : this->shapes_) {
        it->move(offset);
    }
    for (auto &it : this->lights_) {
        it->move(offset);
    }
}

void Scene::rotate(const Math::Vector3D &angles) {
    for (auto &it : this->shapes_) {
        it->rotate(angles);
    }
    for (auto &it : this->lights_) {
        it->rotate(angles);
    }
}

void Scene::scale(size_t scale) {
    for (auto &it : this->shapes_) {
        it->scale(scale);
    }
    for (auto &it : this->lights_) {
        it->scale(scale);
    }
}
void Scene::setPosition(const Math::Vector3D &newPos) {
    for (auto &it : this->shapes_) {
        it->setPosition(newPos);
    }
    for (auto &it : this->lights_) {
        it->setPosition(newPos);
    }
}

}  // namespace RayTracer
