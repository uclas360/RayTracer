/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Lambertarian
*/

#include "../include/Dielectric.hpp"

#include <libconfig.h++>

#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

Dielectric::Dielectric(const libconfig::Setting &config) {
    if (!config.lookupValue("refraction_index", this->refraction_index)) {
        throw ParsingException(
            "error parsing dialectic material, missing \"refraction_index\" "
            "field");
    }
}

bool Dielectric::scatter(const Ray &r_in, const HitRecord &rec,
                         Math::Vector3D &attenuation, Ray &scattered) const {
    attenuation = Math::Vector3D(1.0, 1.0, 1.0);
    double ri = rec.frontFace ? (1.0 / refraction_index) : refraction_index;

    Math::Vector3D unit_direction = r_in.dir.normalized();
    double cos_theta = std::fmin((-unit_direction).dot(rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    Math::Vector3D direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
        direction = reflect(unit_direction, rec.normal);
    } else {
        direction = refract(unit_direction, rec.normal, ri);
    }

    scattered = Ray(rec.p, direction);
    return true;
}

std::unique_ptr<Material> Dielectric::duplicate(void) {
    return std::make_unique<Dielectric>(*this);
}

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new Dielectric(config);
}
}
}  // namespace RayTracer
