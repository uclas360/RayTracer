/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Scene
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>
#include "plugins/ILight.hpp"
#include "plugins/IShape.hpp"

// TODO : a Scene class

namespace RayTracer {

class Scene : public IShape {
public:
    HitRecord hits(const Ray &ray) const override;
    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void scale(size_t scale) override;
    void setPosition(const Math::Vector3D &newPos) override;

    void addShape(std::unique_ptr<IShape> shape);
    void addLight(std::unique_ptr<ILight> light);

    std::vector<std::unique_ptr<IShape>> shapes_;
    std::vector<std::unique_ptr<ILight>> lights_;
};

};  // namespace RayTracer

#endif
