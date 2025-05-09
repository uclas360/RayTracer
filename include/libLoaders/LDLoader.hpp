/*
** EPITECH PROJECT, 2025
** raytraycer
** File description:
** lib loaders
*/

#ifndef DL_LOADER_HPP
#define DL_LOADER_HPP

#include <dlfcn.h>
#include <unistd.h>

#include <libconfig.h++>
#include <memory>
#include <string>

#include "libLoaders/ILibLoader.hpp"
#include "plugins/IPlugin.hpp"

template <lib Module>
class DlLoader : public LibLoader<Module> {
 public:
  DlLoader(std::string fileName)
      : lib_(dlopen((fileName + ".so").c_str(), RTLD_LAZY)) {
    if (this->lib_ == nullptr) {
      throw NotExistingLib(dlerror());
    }
  }

  DlLoader(DlLoader<Module> &&other) : lib_(other.lib_) {
    other.lib_ = nullptr;
  }

  DlLoader(DlLoader<Module> &other) = delete;
  ~DlLoader() override {
    if (this->lib_) (void)dlclose(this->lib_);
  }

  std::unique_ptr<Module> getInstance(
      const std::string entryPoint,
      const libconfig::Setting &settings) override {
    IPlugin *(*function)(const libconfig::Setting &) =
        (IPlugin * (*)(const libconfig::Setting &))(
            dlsym(this->lib_, entryPoint.c_str()));

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

  template <class... A>
  std::unique_ptr<Module> getInstance(const std::string entryPoint, A... args) {
    IPlugin *(*function)(A...) =
        (IPlugin * (*)(A...))(dlsym(this->lib_, entryPoint.c_str()));

    if (!function) {
      throw LoaderException("not a raytracerPlugin lib");
    }
    Module *instance = dynamic_cast<Module *>(function(args...));
    if (!instance) {
      throw LoaderException("wrong plugin type");
    }
    std::unique_ptr<Module> tmp(instance);
    return std::move(tmp);
  }

 private:
  void *lib_ = nullptr;
};

#endif
