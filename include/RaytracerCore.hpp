/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** customException base class
*/

#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>

#include "ArgsManager.hpp"
#include "CustomException.hpp"
#include "libLoaders/ILibLoader.hpp"
#include "plugins/ILight.hpp"
#include "plugins/IShape.hpp"
#include "raytracer/Camera.hpp"

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

    RayTracer::Camera _camera_;

    std::map<std::string, std::unique_ptr<LibLoader<IShape>>> _shapesPlugins_;
    std::map<std::string, std::unique_ptr<LibLoader<ILight>>> _lightsPlugins_;

    std::vector<std::unique_ptr<IShape>> _shapes_;
    std::vector<std::unique_ptr<ILight>> _lights_;

    void initPlugins(const std::string &file, const libconfig::Config &config);

    void initCamera(const std::string &file, const libconfig::Config &config,
                    std::optional<RayTracer::Camera> &camera);

    bool _graphic_;
};
