/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** customException base class
*/

#pragma once

#include <map>
#include <memory>
#include <string>
#include "ArgsManager.hpp"
#include "libLoaders/ILibLoader.hpp"
#include "plugins/IShape.hpp"
#include "plugins/ILight.hpp"

class RaytracerCore {
    public:
        RaytracerCore(const ArgManager::ArgumentStruct &);
        void compute(void);
    private:
        void computeGraphic(void);
        void computeOutput(void);

        std::map<std::string, std::unique_ptr<LibLoader<IShape>>> shapesPlugins;
        std::map<std::string, std::unique_ptr<LibLoader<ILight>>> lightsPlugins;
};
