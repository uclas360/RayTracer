/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** interface for plugin loader
*/

#pragma once

#include <memory>
#include "CustomException.hpp"

class NotExistingLib : public CustomException {
   public:
    NotExistingLib(std::string message) : CustomException(message) {}
};

class LoaderNotExistingFunction : public CustomException {
   public:
    LoaderNotExistingFunction(std::string message) : CustomException(message) {}
};

class LoaderException : public std::exception {
   public:
    LoaderException(std::string message) : _message_(message) {};
    const char *what() const noexcept override {
        return this->_message_.c_str();
    }

   private:
    std::string _message_;
};

template <typename Module>
class LibLoader {
   public:
    virtual ~LibLoader() = default;
    virtual std::unique_ptr<Module> getInstance(const std::string entryPoint) = 0;
};
