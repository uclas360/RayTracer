/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** Ray
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <libconfig.h++>

#include "Raytracer/math/Vector.hpp"
#include "plugins/IObject.hpp"

namespace RayTracer {

class Camera : public IObject {
   public:
    Camera() = default;
    Camera(libconfig::Setting &);
    Camera(Camera &&);

    void move(const Math::Vector3D &offset) override;
    void rotate(const Math::Vector3D &angles) override;
    void scale(size_t scale) override;
    void setPosition(const Math::Vector3D &newPos) override;

   private:
    Math::Vector3D pos_;
};

}  // namespace RayTracer

#endif
