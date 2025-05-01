/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** arg manager
*/

#ifndef ARGS_MANAGER_HPP
#define ARGS_MANAGER_HPP

#include <functional>
#include <map>

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
        bool graphicMode = false;
    };

    ArgManager(char **argv);

    const ArgumentStruct &getArguments(void) const noexcept;

   private:
    ArgumentStruct arguments_;
    std::map<std::string, std::function<void(ArgManager &)>> flagMap = {
        {"-g", [](ArgManager &_this) {
             if (!_this.arguments_.graphicMode) {
                 _this.arguments_.graphicMode = true;
             } else {
                 throw ArgumentException("multiple use of -g flag");
             }
         }}};
};

#endif
