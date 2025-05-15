/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** IObject
*/

#ifndef IOBJECT_HPP
#define IOBJECT_HPP

#include <cstddef>
#include <memory>

#include "Raytracer/math/Vector.hpp"
#include "plugins/IPlugin.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class IObject : public IPlugin {
 public:
  virtual void move(const Math::Vector3D &offset) = 0;
  virtual void rotate(const Math::Vector3D &angles) = 0;
  virtual void scale(size_t scale) = 0;
  virtual void setPosition(const Math::Vector3D &newPos) = 0;
};

};  // namespace RayTracer

#endif
