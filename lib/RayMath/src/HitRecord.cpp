/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** HitRecord
*/

#include "plugins/IShape.hpp"

namespace RayTracer {

HitRecord::HitRecord(const double t, const Ray &ray, const IShape &shape,
                     const Math::Vector3D &normal,
                     const std::unique_ptr<Material> &mat) {
    this->missed = false;
    this->t = t;
    this->frontFace = ray.dir.dot(normal) < 0;
    this->normal = this->frontFace ? normal : -normal;
    this->p = ray.at(t) + this->normal * 1E-4;
    this->shapeHit = shape;
    this->mat = mat;
}

std::ostream &operator<<(std::ostream &out, const HitRecord &vec) {
    std::cout << "missed: " << vec.missed << std::endl;
    std::cout << "frontface: " << vec.frontFace << std::endl;
    std::cout << "t: " << vec.t << std::endl;
    std::cout << "p: " << vec.p << std::endl;
    std::cout << "normal: " << vec.normal << std::endl;
    return out;
}

}  // namespace RayTracer
