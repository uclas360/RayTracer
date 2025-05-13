/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Camera
*/

#include "Raytracer/Camera.hpp"

#include <utility>
#include <cmath>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"

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

RayTracer::Ray RayTracer::Camera::ray(double u, double v) {
  return Ray(this->pos_, this->screen_.pointAt(u, v) - this->pos_);
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

void RayTracer::Camera::scale(size_t scale) { (void)scale; }

void RayTracer::Camera::setPosition(const Math::Vector3D &newPos) {
  Math::Vector3D offset = newPos - this->pos_;
  this->pos_ += offset;
  this->screen_.pos += offset;
}

void RayTracer::Camera::lookAt(const Math::Vector3D &target) {
}
