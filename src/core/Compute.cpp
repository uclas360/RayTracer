/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core compute
*/

#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <array>
#include <cstdint>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"
#include "Utils.hpp"

#define CAM_SPEED 0.03
#define LIGHT_REFLEXION 0.6

struct rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

void setPixelColor(uint8_t *array, int index, rgb color) {
  array[index * 4] = color.r;
  array[index * 4 + 1] = color.g;
  array[index * 4 + 2] = color.b;
  array[index * 4 + 3] = 255;
}

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

uint8_t *RaytracerCore::generateImage(int width, int height) {
  uint8_t *array =
      static_cast<uint8_t *>(malloc(sizeof(uint8_t) * (width * height * 4)));
  double screenWidth = width;
  double screenHeight = height;
  Math::Vector3D vec;
  Math::Vector3D lightPos = {0, -2, -1};
  RayTracer::HitRecord hitRecord;

  for (double y = 0; y < screenHeight; y++) {
    for (double x = 0; x < screenWidth; x++) {
      double u = x / screenWidth;
      double v = y / screenHeight;
      RayTracer::Ray r = this->camera_.ray(u, v);

      hitRecord = this->mainScene_.hits(r);
      if (!hitRecord.missed) {
      }
      vec = ray_color(r, 2, this->mainScene_);
      setPixelColor(array, y * width + x,
                    {static_cast<unsigned char>(vec.x * 255),
                     static_cast<unsigned char>(vec.y * 255),
                     static_cast<unsigned char>(vec.z * 255)});

      Math::Vector3D toLight = lightPos - hitRecord.p;

      if (!hitRecord.missed) {
        float lightEfficiency = hitRecord.normal.dot(toLight.normalized());
        RayTracer::HitRecord lightHitRecord = this->mainScene_.hits(
            RayTracer::Ray(hitRecord.p + hitRecord.normal * EPSILON, toLight));

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
  }
  return array;
}

void RaytracerCore::compute() {
  int screenWidth = 800;
  int screenHeight = 800;
  sf::Clock clock;
  sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight),
                          "SFML window");

  const unsigned int W = 80;
  const unsigned int H = 80;

  sf::Uint8 *pixels;
  sf::Texture texture;
  texture.create(W, H);
  sf::Sprite sprite(texture);
  sprite.setScale(screenWidth / W, screenHeight / H);

  while (window.isOpen()) {
    if (clock.getElapsedTime().asSeconds() < 1.0 / 60.0) continue;
    clock.restart();

    pixels = this->generateImage(W, H);
    texture.update(pixels);

    sf::Event event;
    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
      this->camera_.move({-CAM_SPEED, 0, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      this->camera_.move({CAM_SPEED, 0, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
      this->camera_.move({0, -CAM_SPEED, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      this->camera_.move({0, CAM_SPEED, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
      this->camera_.move({0, 0, -CAM_SPEED});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      this->camera_.move({0, 0, CAM_SPEED});
    Math::Vector3D camera_Rotation(0, 0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
      camera_Rotation.y += 0.1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
      camera_Rotation.y -= 0.1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
      camera_Rotation.x += 0.1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
      camera_Rotation.x -= 0.1;
    }
    this->camera_.rotate(camera_Rotation);

    window.clear();
    window.draw(sprite);
    window.display();
  }
}
