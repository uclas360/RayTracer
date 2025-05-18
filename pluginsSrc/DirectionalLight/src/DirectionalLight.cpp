/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Lambertarian
*/

#include "../include/DirectionalLight.hpp"

#include <iostream>
#include <libconfig.h++>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {

DirectionalLight::DirectionalLight(const libconfig::Setting &config) {
    libconfig::Setting &dir = config.lookup("dir");

    if (!config.lookupValue("red", this->albedo.x)) {
        throw ParsingException(
            "error parsing directionalLight material, missing \"red\" field");
    }
    if (!config.lookupValue("green", this->albedo.y)) {
        throw ParsingException(
            "error parsing directionalLight material, missing \"green\" field");
    }
    if (!config.lookupValue("blue", this->albedo.z)) {
        throw ParsingException(
            "error parsing directionalLight material, missing \"blue\" field");
    }
    if (!Math::lookUpVector(dir, this->dir))
        throw ParsingException(
            "error parsing directional light object, wrong \"dir\" field");
    this->albedo.setRange(255, 1);
    this->dir.normalize();
}

void DirectionalLight::save(libconfig::Setting &parent) const {
    libconfig::Setting &mat =
        parent.add("material", libconfig::Setting::TypeGroup);
    mat.add("type", libconfig::Setting::TypeString) = "DirectionalLight";
    libconfig::Setting &data = mat.add("data", libconfig::Setting::TypeGroup);

    data.add("red", libconfig::Setting::TypeFloat) = this->albedo.x * 255;
    data.add("green", libconfig::Setting::TypeFloat) = this->albedo.y * 255;
    data.add("blue", libconfig::Setting::TypeFloat) = this->albedo.z * 255;
}

bool DirectionalLight::scatter(const Ray &, const HitRecord &rec,
                               Math::Vector3D &attenuation,
                               Ray &scattered) const {
    Math::Vector3D scatter_direction =
        rec.normal + Math::Vector3D::random_unit_vector();

    if (scatter_direction.near_zero()) scatter_direction = rec.normal;
    scattered = Ray(rec.p, scatter_direction);
    attenuation = this->albedo;
    return true;
}

void DirectionalLight::setColor(const Math::Vector3D &newColor) {
    this->albedo = newColor;
    this->albedo.setRange(255, 1);
}

Math::Vector3D DirectionalLight::emitted(const Ray &, const HitRecord &rec) const {
    return (this->albedo * rec.normal.normalized().dot(this->dir)).clamped(0.0, 1.0);
}

std::unique_ptr<Material> DirectionalLight::duplicate(void) {
    return std::make_unique<DirectionalLight>(*this);
}

extern "C" {
void *entry_point(const libconfig::Setting &config) {
    return new DirectionalLight(config);
}
}
}  // namespace RayTracer
