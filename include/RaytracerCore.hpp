/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** customException base class
*/

#ifndef RAYTRACER_CORE_HPP
#define RAYTRACER_CORE_HPP

#include <SFML/Window/Keyboard.hpp>
#include <mutex>
#include "Raytracer/math/Vector.hpp"
#define CAM_SPEED 0.03
#define LIGHT_REFLEXION 0.6

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <thread>

#include "ArgsManager.hpp"
#include "CustomException.hpp"
#include "Raytracer/Camera.hpp"
#include "Raytracer/Scene.hpp"
#include "libLoaders/ILibLoader.hpp"
#include "plugins/ILight.hpp"
#include "plugins/IShape.hpp"

class ParsingException : public CustomException {
   public:
    ParsingException(std::string message) : CustomException(message) {
    }
};

class RaytracerCore {
   public:
    RaytracerCore(const ArgManager::ArgumentStruct &);
    void compute(void);

    RayTracer::Camera &getCam(void) {return camera_;};
    RayTracer::Scene &getMainScene(void) {return mainScene_;};
    void setMoving(bool moving) {moving_ = moving;};


   private:
    void computeGraphic(void);
    void computeOutput(void);

    void computeImage(size_t start, size_t end);
    void computePixel(std::vector<std::uint8_t> &image, size_t pixel, size_t xResolution, size_t yResolution);

    RayTracer::Camera camera_;

    std::map<std::string, std::unique_ptr<LibLoader<RayTracer::IShape>>> shapesPlugins_;
    std::map<std::string, std::unique_ptr<LibLoader<RayTracer::ILight>>> lightsPlugins_;

    RayTracer::Scene mainScene_;

    void initPlugins(const std::string &file, const libconfig::Config &config);

    void initCamera(const std::string &file, const libconfig::Config &config,
                    std::optional<RayTracer::Camera> &camera);

    bool graphic_;

    std::vector<std::thread> threads_;
    bool killThreads_ = false;
    void startThreads(size_t nbThreads);

    void computeMoving(size_t start, size_t end);
    void computePrecision(void);

    std::mutex imageMutex_;
    size_t nbImage_ = 0;
    std::vector<std::uint8_t> imageMean_;
    size_t width_;
    size_t height_;
    size_t xResolution_;
    size_t yResolution_;

    size_t compressedXResolution_;
    size_t compressedYResolution_;
    std::vector<std::uint8_t> compressedImage_;

    bool moving_ = false;

    void handleKeys(void);
    const std::map<sf::Keyboard::Key, std::function<void(Math::Vector3D &)>> keyboardEvent{
        {sf::Keyboard::Q, [this](Math::Vector3D &) {this->camera_.move({-CAM_SPEED, 0, 0});}},
        {sf::Keyboard::D, [this](Math::Vector3D &) {this->camera_.move({CAM_SPEED, 0, 0});}},
        {sf::Keyboard::Z, [this](Math::Vector3D &) {this->camera_.move({0, -CAM_SPEED, 0});}},
        {sf::Keyboard::S, [this](Math::Vector3D &) {this->camera_.move({0, CAM_SPEED, 0});}},
        {sf::Keyboard::E, [this](Math::Vector3D &) {this->camera_.move({0, 0, -CAM_SPEED});}},
        {sf::Keyboard::A, [this](Math::Vector3D &) {this->camera_.move({0, 0, CAM_SPEED});}},
        {sf::Keyboard::Left, [](Math::Vector3D &camRotation) {camRotation.y += 0.1;}},
        {sf::Keyboard::Right, [](Math::Vector3D &camRotation) {camRotation.y -= 0.1;}},
        {sf::Keyboard::P, [this](Math::Vector3D &) {this->camera_.lookAt({0, 0, -2});}},
    };
};

#endif
