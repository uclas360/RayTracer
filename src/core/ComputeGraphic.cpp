/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core compute
*/

#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <vector>

#include "Raytracer/math/Vector.hpp"
#include "RaytracerCore.hpp"

void RaytracerCore::computeGraphic() {
  sf::Clock clock;
  sf::RenderWindow window(sf::VideoMode(this->width_, this->height_),
                          "Raytracer");

  sf::Texture texture;
  texture.create(this->xResolution_, this->yResolution_);
  sf::Sprite sprite(texture);
  sprite.setScale((double)this->width_ / this->xResolution_, (double)this->height_ / this->yResolution_);

  while (window.isOpen()) {
    if (clock.getElapsedTime().asSeconds() < 1.0 / 60.0) continue;
    clock.restart();

    texture.update(this->image_.data());

    sf::Event event;
    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
      this->mainScene_.shapes_[4]->rotate({0, -CAM_SPEED, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
    this->mainScene_.shapes_[4]->rotate({0, CAM_SPEED, 0});
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      camera_Rotation.y += 0.1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      camera_Rotation.y -= 0.1;
    }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    //   camera_Rotation.x += 0.1;
    // }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    //   camera_Rotation.x -= 0.1;
    // }
    this->camera_.rotate(camera_Rotation);

    window.clear();
    window.draw(sprite);
    window.display();
  }
  this->graphic_ = false;
  for (auto &it : this->threads_) {
      it.join();
  }
}
