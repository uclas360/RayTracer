/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** AObject
*/

#ifndef AOBJECT_HPP
#define AOBJECT_HPP

#include <memory>

#include "AABB.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class AShape : public IShape {
   public:
    virtual void setMaterial(std::unique_ptr<Material> &) override;
    const AABB &boundingBox() const override;

   protected:
    int depth = 0;
    AABB bbox;
    std::unique_ptr<Material> material_;
};

};  // namespace RayTracer

#endif
