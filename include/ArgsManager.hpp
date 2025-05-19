/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** arg manager
*/

#ifndef ARGS_MANAGER_HPP
#define ARGS_MANAGER_HPP

#include <sys/types.h>

#include <cstdlib>
#include <functional>
#include <map>
#include <sstream>

#include "CustomException.hpp"

class ArgumentException : public CustomException {
   public:
    ArgumentException(std::string message) : CustomException(message) {
    }
};

class ArgManager {
   public:
    struct ArgumentStruct {
        std::vector<std::string> configFile;
        ssize_t nb_thread = -1;
        bool graphicMode = false;
        ssize_t width = -1;
        ssize_t height = -1;
        ssize_t xResolution = -1;
        ssize_t yResolution = -1;
        ssize_t finalNbImages = -1;
    };

    ArgManager(char **argv);

    const ArgumentStruct &getArguments(void) const noexcept;

   private:
    ArgumentStruct arguments_;
    std::map<std::string, std::function<bool(ArgManager &, char *)>> flagMap = {
        {"-g",
         [](ArgManager &_this, char *) {
             if (!_this.arguments_.graphicMode) {
                 _this.arguments_.graphicMode = true;
             } else {
                 throw ArgumentException("multiple use of -g flag");
             }
             return false;
         }},
        {"-t",
         [](ArgManager &_this, char *number) {
             if (number == nullptr) {
                 throw ArgumentException(
                     "-t flag must take a positive integer");
             }
             if (_this.arguments_.nb_thread != -1) {
                 throw ArgumentException("multiple use of -g flag");
             }
             std::stringstream ss(number);
             size_t nb_thread;

             ss >> nb_thread;
             if (ss.fail() || !ss.eof()) {
                 throw ArgumentException(
                     "-t flag must take a positive integer");
             }
             _this.arguments_.nb_thread = nb_thread;
             return true;
         }},
        {"-W",
         [](ArgManager &_this, char *number) {
             if (number == nullptr) {
                 throw ArgumentException(
                     "-W flag must take a positive integer");
             }
             if (_this.arguments_.width != -1) {
                 throw ArgumentException("multiple use of -W flag");
             }
             std::stringstream ss(number);
             size_t width;

             ss >> width;
             if (ss.fail() || !ss.eof()) {
                 throw ArgumentException(
                     "-W flag must take a positive integer");
             }
             _this.arguments_.width = width;
             return true;
         }},
        {"-H",
         [](ArgManager &_this, char *number) {
             if (number == nullptr) {
                 throw ArgumentException(
                     "-H flag must take a positive integer");
             }
             if (_this.arguments_.height != -1) {
                 throw ArgumentException("multiple use of -H flag");
             }
             std::stringstream ss(number);
             size_t height;

             ss >> height;
             if (ss.fail() || !ss.eof()) {
                 throw ArgumentException(
                     "-H flag must take a positive integer");
             }
             _this.arguments_.height = height;
             return true;
         }},
        {"-xr",
         [](ArgManager &_this, char *number) {
             if (number == nullptr) {
                 throw ArgumentException(
                     "-xr flag must take a positive integer");
             }
             if (_this.arguments_.xResolution != -1) {
                 throw ArgumentException("multiple use of -xr flag");
             }
             std::stringstream ss(number);
             size_t xRes;

             ss >> xRes;
             if (ss.fail() || !ss.eof()) {
                 throw ArgumentException(
                     "-xr flag must take a positive integer");
             }
             _this.arguments_.xResolution = xRes;
             return true;
         }},
        {"-yr", [](ArgManager &_this, char *number) {
             if (number == nullptr) {
                 throw ArgumentException(
                     "-xr flag must take a positive integer");
             }
             if (_this.arguments_.yResolution != -1) {
                 throw ArgumentException("multiple use of -xr flag");
             }
             std::stringstream ss(number);
             size_t yRes;

             ss >> yRes;
             if (ss.fail() || !ss.eof()) {
                 throw ArgumentException(
                     "-yr flag must take a positive integer");
             }
             _this.arguments_.yResolution = yRes;
             return true;
         }},
        {"-images", [](ArgManager &_this, char *number) {
             if (number == nullptr) {
                 throw ArgumentException(
                     "-images flag must take a positive integer");
             }
             if (_this.arguments_.yResolution != -1) {
                 throw ArgumentException("multiple use of -images flag");
             }
             std::stringstream ss(number);
             size_t nbimages;

             ss >> nbimages;
             if (ss.fail() || !ss.eof()) {
                 throw ArgumentException(
                     "-yr flag must take a positive integer");
             }
             _this.arguments_.finalNbImages = nbimages;
             return true;
         }}
        };
};

#endif
