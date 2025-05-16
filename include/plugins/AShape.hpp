/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** AObject
*/

#ifndef AOBJECT_HPP
#define AOBJECT_HPP

#include <memory>

#include "plugins/Material.hpp"
#include "Raytracer/Texture.hpp"
#include "plugins/IShape.hpp"
#include "AABB.hpp"


namespace RayTracer {

class AABB;

class AShape : public IShape {
   public:
    virtual void setMaterial(std::unique_ptr<Material> &) override;
    const AABB &boundingBox() const override;
    virtual std::unique_ptr<Material> &getMaterial() override;
    bool haveTexture() const;


   protected:
    AABB bbox;
    std::unique_ptr<Material> material_;
    Texture texture_;
};

};  // namespace RayTracer

#endif
