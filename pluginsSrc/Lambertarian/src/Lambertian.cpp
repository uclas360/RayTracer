/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Lambertarian
*/

#include "../include/Lambertian.hpp"
#include <libconfig.h++>

#include "Raytracer/math/Color.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Lambertian::Lambertian(const libconfig::Setting &config) {
    double r;
    double g;
    double b;

    if (!config.lookupValue("red", r)) {
      throw ParsingException(
          "error parsing lambertian material, missing \"red\" field");
    }
    if (!config.lookupValue("green", g)) {
      throw ParsingException(
          "error parsing lambertian material, missing \"green\" field");
    }
    if (!config.lookupValue("blue", b)) {
      throw ParsingException(
          "error parsing lambertian material, missing \"blue\" field");
    }
    this->albedo = {(double)COLOR_RANGE((int)r), (double)COLOR_RANGE((int)g), (double)COLOR_RANGE((int)b)};
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

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new Lambertian(config);
}
}
}  // namespace RayTracer
