/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** AObject
*/

#ifndef AOBJECT_HPP
#define AOBJECT_HPP

#include <memory>

#include "plugins/IShape.hpp"
#include "plugins/Material.hpp"

namespace RayTracer {

class AShape : public IShape {
    public:
    void setMaterial(Material &) override;
    protected:
    Material material_;
};

};  // namespace RayTracer

#endif
