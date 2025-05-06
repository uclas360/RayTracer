/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core compute
*/

#include <vector>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "Utils.hpp"
#include "plugins/IShape.hpp"

Math::Vector3D getSkyColor(const RayTracer::Ray &r) {
  Math::Vector3D unit_direction = r.dir.normalized();
  double a = 0.5 * (unit_direction.y + 1.0);
  return Math::Vector3D(0.9, 0.9, 0.9) * (1.0 - a) +
         Math::Vector3D(0.3, 0.5, 1.0) * a;
}

Math::Vector3D ray_color(const RayTracer::Ray &r, int depth,
                         const RayTracer::Scene &scene) {
  if (depth <= 0) return Math::Vector3D(0, 0, 0);

  RayTracer::HitRecord rec = scene.hits(r);

  if (!rec.missed) {
    Math::Vector3D direction = Math::Vector3D::random_on_hemisphere(rec.normal);
    return ray_color(RayTracer::Ray(rec.p + rec.normal * EPSILON, direction),
                     depth - 1, scene) *
           LIGHT_REFLEXION;
  }

  Math::Vector3D unit_direction = r.dir.normalized();
  double a = 0.5 * (unit_direction.y + 1.0);
  return Math::Vector3D(0.9, 0.9, 0.9) * (1.0 - a) +
         Math::Vector3D(0.3, 0.5, 1.0) * a;
}

void RaytracerCore::computePixel(size_t pixel) {
    size_t y = pixel / this->xResolution_;
    size_t x = pixel % this->xResolution_;
    double u = (double)x / this->xResolution_;
    double v = (double)y / this->yResolution_;
    RayTracer::Ray r = this->camera_.ray(u, v);
    RayTracer::HitRecord hitRecord;
    Math::Vector3D vec;
    Math::Vector3D lightPos = {0, -2, -1};

    hitRecord = this->mainScene_.hits(r);
    if (!hitRecord.missed) {
    }
    vec = ray_color(r, 2, this->mainScene_);
    this->image_[pixel * 4] = static_cast<unsigned char>(vec.x * 255);
    this->image_[pixel * 4 + 1] = static_cast<unsigned char>(vec.y * 255);
    this->image_[pixel * 4 + 2] = static_cast<unsigned char>(vec.z * 255);
    this->image_[pixel * 4 + 3] = 255;

    // Math::Vector3D toLight = lightPos - hitRecord.p;

    if (!hitRecord.missed) {
      //float lightEfficiency = hitRecord.normal.dot(toLight.normalized());
      //RayTracer::HitRecord lightHitRecord = this->mainScene_.hits(
      //    RayTracer::Ray(hitRecord.p + hitRecord.normal * EPSILON, toLight));

      // if (lightHitRecord.missed)
      //     setPixelColor(array, y * width + x,
      //                   {static_cast<unsigned char>(
      //                        std::max(0.0f, lightEfficiency *
      //                        255)),
      //                    static_cast<unsigned char>(
      //                        std::max(0.0f, lightEfficiency *
      //                        255)),
      //                    static_cast<unsigned char>(
      //                        std::max(0.0f, lightEfficiency *
      //                        255))});
      // else
      //     setPixelColor(array, y * width + x, {0, 0, 0});
      // }
    }
}

void RaytracerCore::computeImage(size_t start, size_t end) {
    do {
        for (size_t i = start; i < end; i++) {
            this->computePixel(i);
        }
    } while (this->graphic_);
}

void RaytracerCore::compute(void) {
    if (this->graphic_) {
        this->computeGraphic();
    } else {
        this->computeOutput();
    }
}
