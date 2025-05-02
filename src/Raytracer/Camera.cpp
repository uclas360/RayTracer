/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Camera
*/

#include "Raytracer/Camera.hpp"

#include <utility>

#include "Raytracer/Ray.hpp"
#include "RaytracerCore.hpp"

RayTracer::Camera::Camera(libconfig::Setting &settings): screen_(Math::Vector3D(-0.5, -0.5, -1), Math::Vector3D(1, 0, 0), Math::Vector3D(0, 1, 0)) {
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

RayTracer::Ray RayTracer::Camera::ray(double u, double v) {
    return Ray(this->pos_, this->screen_.pointAt(u, v) - this->pos_);
}

void RayTracer::Camera::move(const Math::Vector3D &offset) {
    this->pos_ += offset;
}

void RayTracer::Camera::rotate(const Math::Vector3D &angles) {
    this->rotateX(angles.x);
    this->rotateY(angles.y);
    this->rotateZ(angles.z);
}

void RayTracer::Camera::rotateX(double angle) {
    Math::Vector3D toScreen = this->screen_.pos - this->pos_;

    this->rotationX += angle;
    this->screen_.pos -= toScreen;
    toScreen.rotateX(angle);
    this->screen_.pos += toScreen;
    this->screen_.bottomSide.rotateX(angle);
}

void RayTracer::Camera::rotateY(double angle) {
    Math::Vector3D toScreen = this->screen_.pos - this->pos_;

    this->rotationY += angle;
    this->screen_.pos -= toScreen;
    toScreen.rotateY(angle);
    this->screen_.pos += toScreen;
    this->screen_.bottomSide.rotateY(angle);
}

void RayTracer::Camera::rotateZ(double angle) {
    Math::Vector3D toScreen = this->screen_.pos - this->pos_;

    this->rotationZ += angle;
    this->screen_.pos -= toScreen;
    toScreen.rotateZ(angle);
    this->screen_.pos += toScreen;
    this->screen_.bottomSide.rotateZ(angle);
}

void RayTracer::Camera::scale(size_t scale) {
    (void)scale;
}

void RayTracer::Camera::setPosition(const Math::Vector3D &newPos) {
    this->pos_ = newPos;
}
