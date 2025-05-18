/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Camera
*/

#include "Raytracer/Camera.hpp"

#include <cmath>
#include <libconfig.h++>
#include <utility>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"

Math::Vector3D sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit
    // square.
    return Math::Vector3D(random_double() - 0.5, random_double() - 0.5, 0);
}

RayTracer::Camera::Camera(libconfig::Setting &settings)
    : screen_(Math::Vector3D(-0.5, -0.5, -1), Math::Vector3D(1, 0, 0),
              Math::Vector3D(0, 1, 0)) {
    try {
        libconfig::Setting &pos = settings.lookup("pos");
        if (!Math::lookUpVector(pos, this->pos_)) {
            throw ParsingException(
                "error parsing cylinder object, wrong \"pos\" field");
        }
    } catch (const ParsingException &e) {
        throw e;
    } catch (const libconfig::SettingNotFoundException &e) {
        throw ParsingException(e.what());
    }
    screen_.pos = {this->pos_.x - 0.5, this->pos_.y - 0.5, this->pos_.z - 1};
};

RayTracer::Ray RayTracer::Camera::ray(double u, double v, double xResolution,
                                      double yResolution) {
    v = -v + 1;
    Math::Vector3D offset = sample_square();
    Math::Vector3D pixel_sample =
        this->screen_.pos +
        this->screen_.bottomSide * (u + (offset.x / xResolution)) +
        this->screen_.leftSide * (v + (offset.y / yResolution));

    return Ray(this->pos_, pixel_sample - this->pos_);
}

void RayTracer::Camera::move(const Math::Vector3D &offset) {
    this->setPosition(this->pos_ + offset.rotatedX(this->rotationX)
                                       .rotatedY(this->rotationY)
                                       .rotatedZ(this->rotationZ));
}

void RayTracer::Camera::rotate(const Math::Vector3D &angles) {
    if (angles.x) this->rotateX(angles.x);
    if (angles.y) this->rotateY(angles.y);
    if (angles.z) this->rotateZ(angles.z);
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
    Math::Vector3D offset = newPos - this->pos_;
    this->pos_ += offset;
    this->screen_.pos += offset;
}

void RayTracer::Camera::lookAt(const Math::Vector3D &) {}

void RayTracer::Camera::save(libconfig::Setting &parent) const {
  libconfig::Setting &pos = parent.add("pos", libconfig::Setting::TypeGroup);
  Math::writeUpVector(pos, this->pos_);
  libconfig::Setting &rotation =
      parent.add("rotation", libconfig::Setting::TypeGroup);
  Math::writeUpVector(rotation, this->rotation_);
}
