/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Camera
*/

#include "Raytracer/Camera.hpp"

#include <utility>

#include "RaytracerCore.hpp"

RayTracer::Camera::Camera(libconfig::Setting &settings) {
    double posX;
    double posY;
    double posZ;

    if (!settings.lookupValue("posX", posX)) {
        throw ParsingException("error creating camera, posX field missing");
    }
    if (!settings.lookupValue("posY", posY)) {
        throw ParsingException("error creating camera, posY field missing");
    }
    if (!settings.lookupValue("posZ", posZ)) {
        throw ParsingException("error creating camera, posZ field missing");
    }
    this->pos_ = {posX, posY, posZ};
};

RayTracer::Camera::Camera(RayTracer::Camera &&other)
    : pos_(std::move(other.pos_)) {
}

void RayTracer::Camera::move(const Math::Vector3D &offset) {
    this->pos_ += offset;
}

void RayTracer::Camera::rotate(const Math::Vector3D &angles) {
    (void)angles;
}
void RayTracer::Camera::scale(size_t scale) {
    (void)scale;
}

void RayTracer::Camera::setPosition(const Math::Vector3D &newPos) {
    this->pos_ = newPos;
}
