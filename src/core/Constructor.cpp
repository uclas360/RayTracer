/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core constructor
*/

#include <iostream>
#include <libconfig.h++>
#include <memory>
#include <optional>

#include "RaytracerCore.hpp"
#include "libLoaders/LDLoader.hpp"
#include "plugins/ILight.hpp"
#include "raytracer/Camera.hpp"

void RaytracerCore::initCamera(const std::string &file,
                               const libconfig::Config &config,
                               std::optional<RayTracer::Camera> &camera) {
    try {
        libconfig::Setting &cameraSettings = config.lookup("camera");
        if (camera.has_value()) {
            throw ParsingException(
                "camera object cannot be declared twice. redeclared in file " +
                file + ", ignored.");
        }
        camera.emplace(cameraSettings);
    } catch (const libconfig::SettingNotFoundException &) {
    } catch (const ParsingException &exc) {
        std::cerr << exc.what() << std::endl;
    }
}

template <typename Plugin>
static void initInMap(
    std::map<std::string, std::unique_ptr<LibLoader<Plugin>>> &map,
    const std::string &name, const std::string &file) {
    if (map.contains(name)) {
        return;
    }
#if defined __linux__
    try {
        map.insert({name, std::make_unique<DlLoader<Plugin>>(name)});
    } catch (const NotExistingLib &ex) {
        std::cerr << "error parsing object in file \"" << file
                  << "\", error loading pluggin :" << ex.what() << std::endl;
    }
#endif
}

void RaytracerCore::initPlugins(const std::string &file,
                                const libconfig::Config &config) {
    try {
        libconfig::Setting &objects = config.lookup("objects");
        for (auto it = objects.begin(); it != objects.end(); it++) {
            std::string type;
            std::string name;
            if (!it->lookupValue("type", type)) {
                std::cerr << "error parsing object in file \"" << file
                          << "\", missing or invalid \"type\" field"
                          << std::endl;
                return;
            }
            if (!it->lookupValue("name", name)) {
                std::cerr << "error parsing object in file \"" << file
                          << "\", missing or invalid \"name\" field"
                          << std::endl;
                return;
            }
            name = "plugin/" + name;
            if (type == "shape") {
                initInMap<IShape>(this->_shapesPlugins_, name, file);
                if (this->_shapesPlugins_.find(name) !=
                    this->_shapesPlugins_.end()) {
                    this->_shapes_.emplace_back(
                        this->_shapesPlugins_.at(name)->getInstance(
                            "entry_point", it->lookup("data")));
                }
            } else if (type == "light") {
                initInMap<ILight>(this->_lightsPlugins_, name, file);
            } else {
                std::cerr << "error parsing object in file \"" << file
                          << "\", object type \"" << type << "\" does not exist"
                          << std::endl;
            }
        }
    } catch (const libconfig::SettingNotFoundException &) {
    }
}

RaytracerCore::RaytracerCore(const ArgManager::ArgumentStruct &args)
    : _graphic_(args.graphicMode) {
    std::optional<RayTracer::Camera> camera = std::nullopt;
    libconfig::Config config;

    config.setOptions(libconfig::Config::OptionAutoConvert |
                      libconfig::Config::OptionAllowScientificNotation);
    for (const std::string &file : args.configFile) {
        try {
            config.readFile(file);
            this->initCamera(file, config, camera);
            this->initPlugins(file, config);
        } catch (const libconfig::FileIOException &exc) {
            std::cerr << "error parsing file \"" << file
                      << "\", failed to open file" << std::endl;
        } catch (const libconfig::ParseException &) {
            std::cerr << "error parsing file \"" << file << "\", invalid format"
                      << std::endl;
        }
    }
}
