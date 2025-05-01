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

    RayTracer::Camera camera_;

    std::map<std::string, std::unique_ptr<LibLoader<IShape>>> shapesPlugins_;
    std::map<std::string, std::unique_ptr<LibLoader<ILight>>> lightsPlugins_;

    std::vector<std::unique_ptr<IShape>> shapes_;
    std::vector<std::unique_ptr<ILight>> lights_;

    void initPlugins(const std::string &file, const libconfig::Config &config);

    void initCamera(const std::string &file, const libconfig::Config &config,
                    std::optional<RayTracer::Camera> &camera);

    bool graphic_;
};

#endif
