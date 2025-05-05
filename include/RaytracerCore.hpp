/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** customException base class
*/

#ifndef RAYTRACER_CORE_HPP
#define RAYTRACER_CORE_HPP

#include <map>
#include <memory>
#include <optional>
#include <string>

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

    uint8_t *generateImage(int width, int height);

    RayTracer::Camera camera_;

    std::map<std::string, std::unique_ptr<LibLoader<RayTracer::IShape>>> shapesPlugins_;
    std::map<std::string, std::unique_ptr<LibLoader<RayTracer::ILight>>> lightsPlugins_;

    RayTracer::Scene mainScene_;

    void initPlugins(const std::string &file, const libconfig::Config &config);

    void initCamera(const std::string &file, const libconfig::Config &config,
                    std::optional<RayTracer::Camera> &camera);

    bool graphic_;
};

#endif
