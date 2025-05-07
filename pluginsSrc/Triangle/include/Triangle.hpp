/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle
*/

#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include <libconfig.h++>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

class Triangle : public IShape {
 public:
  Math::Vector3D a;
  Math::Vector3D b;
  Math::Vector3D c;

  Triangle() = default;
  ~Triangle() = default;
  Triangle(Math::Vector3D a, Math::Vector3D b, Math::Vector3D c)
      : a(a), b(b), c(c) {};
  Triangle(const libconfig::Setting &settings);
  HitRecord hits(const Ray &ray) const override;
  void move(const Math::Vector3D &offset) {};
  void rotate(const Math::Vector3D &angles) {};
  void scale(size_t scale) {};
  void setPosition(const Math::Vector3D &newPos) {};

 protected:
 private:
};
}  // namespace RayTracer

#endif /* !TRIANGLE_HPP_ */