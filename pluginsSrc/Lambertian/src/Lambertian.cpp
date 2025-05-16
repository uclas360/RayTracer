/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Lambertarian
*/

#include "../include/Lambertian.hpp"

#include <libconfig.h++>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Lambertian::Lambertian(const libconfig::Setting &config) {
    if (!config.lookupValue("red", this->albedo.x)) {
        throw ParsingException(
            "error parsing lambertian material, missing \"red\" field");
    }
    if (!config.lookupValue("green", this->albedo.y)) {
        throw ParsingException(
            "error parsing lambertian material, missing \"green\" field");
    }
    if (!config.lookupValue("blue", this->albedo.z)) {
        throw ParsingException(
            "error parsing lambertian material, missing \"blue\" field");
    }
    this->albedo.setRange(255, 1);
}

bool Lambertian::scatter(const Ray &, const HitRecord &rec,
                         Math::Vector3D &attenuation, Ray &scattered) const {
    Math::Vector3D scatter_direction =
        rec.normal + Math::Vector3D::random_unit_vector();

    if (scatter_direction.near_zero()) scatter_direction = rec.normal;
    scattered = Ray(rec.p, scatter_direction);
    attenuation = this->albedo;
    return true;
}

void Lambertian::setColor(const Math::Vector3D &newColor) {
    this->albedo = newColor;
    this->albedo.setRange(255, 1);
}

std::unique_ptr<Material> Lambertian::duplicate(void) {
    return std::make_unique<Lambertian>(*this);
}

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new Lambertian(config);
}
}
}  // namespace RayTracer
