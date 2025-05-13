/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** Ray
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iosfwd>
#include <libconfig.h++>

#include "Raytracer/Ray.hpp"
#include "Raytracer/Rectangle.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IObject.hpp"

namespace RayTracer {

class Camera : public IObject {
 public:
  Camera()
      : screen_(Math::Vector3D(-0.5, -0.5, -1), Math::Vector3D(1, 0, 0),
                Math::Vector3D(0, 1, 0)) {};
  Camera(libconfig::Setting &);

  void move(const Math::Vector3D &offset) override;
  void rotate(const Math::Vector3D &angles) override;
  void scale(size_t scale) override;
  void setPosition(const Math::Vector3D &newPos) override;
  void lookAt(const Math::Vector3D &target);
  void save(libconfig::Setting &parent) const;

  Ray ray(double u, double v);

 private:
  void rotateX(double angle);
  void rotateY(double angle);
  void rotateZ(double angle);

  double rotationX = 0;
  double rotationY = 0;
  double rotationZ = 0;

  Rectangle screen_;
  Math::Vector3D pos_;
  Math::Vector3D rotation_;
};

}  // namespace RayTracer

#endif
