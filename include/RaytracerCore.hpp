/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** customException base class
*/

#ifndef RAYTRACER_CORE_HPP
#define RAYTRACER_CORE_HPP

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

   private:
    void computeGraphic(void);
    void computeOutput(void);

    void computeImage(size_t start, size_t end);
    void computePixel(size_t pixel);

    RayTracer::Camera camera_;

    std::map<std::string, std::unique_ptr<LibLoader<RayTracer::IShape>>> shapesPlugins_;
    std::map<std::string, std::unique_ptr<LibLoader<RayTracer::ILight>>> lightsPlugins_;

    RayTracer::Scene mainScene_;

    void initPlugins(const std::string &file, const libconfig::Config &config);

    void initCamera(const std::string &file, const libconfig::Config &config,
                    std::optional<RayTracer::Camera> &camera);

    bool graphic_;

    std::vector<std::thread> threads_;
    void startThreads(size_t nbThreads, size_t width, size_t height);

    std::vector<std::uint8_t> image_;
    size_t width_;
    size_t height_;
    size_t xResolution_;
    size_t yResolution_;
};

#endif
