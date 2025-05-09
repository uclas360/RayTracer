/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core constructor
*/

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <libconfig.h++>
#include <memory>
#include <optional>
#include <thread>

#include "Raytracer/Camera.hpp"
#include "RaytracerCore.hpp"

#if defined __linux__
#include "libLoaders/LDLoader.hpp"
#endif
#if defined _WIN32
#include "libLoaders/WindowsLoader.hpp"
#endif

#include "plugins/ILight.hpp"

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
    try {
// LINUX
#if defined __linux__
        map.insert({name, std::make_unique<DlLoader<Plugin>>(name)});
#endif
//
// WINDOWS
#if defined _WIN32
        map.insert({name, std::make_unique<WindowsLoader<Plugin>>(name)});
#endif
//
    } catch (const NotExistingLib &ex) {
        std::cerr << "error parsing object in file \"" << file
                  << "\", error loading pluggin :" << ex.what() << std::endl;
    }
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
                initInMap<RayTracer::IShape>(this->shapesPlugins_, name, file);
                if (this->shapesPlugins_.find(name) !=
                    this->shapesPlugins_.end()) {
                    try {
                        this->mainScene_.addShape(
                            this->shapesPlugins_.at(name)->getInstance(
                                "entry_point", it->lookup("data")));
                    } catch (const ParsingException &exp) {
                        std::cerr << exp.what() << std::endl;
                    }
                }
            } else if (type == "light") {
                initInMap<RayTracer::ILight>(this->lightsPlugins_, name, file);
                if (this->shapesPlugins_.find(name) !=
                    this->shapesPlugins_.end()) {
                    try {
                        this->mainScene_.addLight(
                            this->lightsPlugins_.at(name)->getInstance(
                                "entry_point", it->lookup("data")));
                    } catch (const ParsingException &exp) {
                        std::cerr << exp.what() << std::endl;
                    }
                }
            } else {
                std::cerr << "error parsing object in file \"" << file
                          << "\", object type \"" << type << "\" does not exist"
                          << std::endl;
            }
        }
    } catch (const libconfig::SettingNotFoundException &) {
    }
}

void RaytracerCore::startThreads(size_t nbThreads) {
    size_t start = 0;
    size_t nbPixelPerThread = (this->compressedXResolution_ * this->compressedYResolution_) / nbThreads;
    size_t end = nbPixelPerThread;
    for (size_t i = 0; i < nbThreads; i++) {
        if (i + 1 == nbThreads) {
            end = this->compressedXResolution_ * this->compressedYResolution_;
        }
        this->threads_.push_back(
            std::thread(&RaytracerCore::computeImage, this, start, end)
        );
        start += nbPixelPerThread;
        end += nbPixelPerThread;
    }
}

RaytracerCore::RaytracerCore(const ArgManager::ArgumentStruct &args)
    : graphic_(args.graphicMode),
      imageMean_(args.xResolution * args.yResolution * 4, 0),
      width_(args.width),
      height_(args.height),
      xResolution_(args.xResolution),
      yResolution_(args.yResolution),
      compressedXResolution_(std::max((int)(args.xResolution / 10), 20)),
      compressedYResolution_(std::max((int)args.yResolution / 10, 20)),
      compressedImage_(this->compressedXResolution_ * this->compressedYResolution_ * 4, 0){
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
    if (camera.has_value()) {
        this->camera_ = std::move(camera.value());
    }
    this->startThreads(args.nb_thread);
}
