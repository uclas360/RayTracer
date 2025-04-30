/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** lib loaders
*/

#pragma once

#include <dlfcn.h>
#include <unistd.h>

#include <libconfig.h++>
#include <memory>
#include <string>

#include "libLoaders/ILibLoader.hpp"
#include "plugins/IPlugin.hpp"

template <typename Module>
concept lib = std::is_base_of<IPlugin, Module>::value;

template <lib Module>
class DlLoader : public LibLoader<Module> {
   public:
    DlLoader(std::string fileName)
        : _lib_(dlopen((fileName).c_str(), RTLD_LAZY)) {
        if (this->_lib_ == nullptr) {
            throw NotExistingLib(dlerror());
        }
    }

    DlLoader(DlLoader<Module> &&other) : _lib_(other._lib_) {
        other._lib_ = nullptr;
    }

    DlLoader(DlLoader<Module> &other) = delete;
    ~DlLoader() override {
        if (this->_lib_) (void)dlclose(this->_lib_);
    }

    std::unique_ptr<Module> getInstance(
        const std::string entryPoint,
        const libconfig::Setting &settings) override {
        IPlugin *(*function)(const libconfig::Setting &) =
            (IPlugin * (*)(const libconfig::Setting &))(
                dlsym(this->_lib_, entryPoint.c_str()));

        if (!function) {
            throw LoaderException("not a raytracerPlugin lib");
        }
        Module *instance = dynamic_cast<Module *>(function(settings));
        if (!instance) {
            throw LoaderException("wrong plugin type");
        }
        std::unique_ptr<Module> tmp(instance);
        return std::move(tmp);
    }

   private:
    void *_lib_ = nullptr;
};
