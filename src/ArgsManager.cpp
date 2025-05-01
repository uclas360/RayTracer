/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** argManager
*/

#include "ArgsManager.hpp"

#include <stdexcept>

ArgManager::ArgManager(char **argv) {
    for (int i = 1; argv[i]; i++) {
        try {
            this->flagMap.at(argv[i])(*this);
        } catch (const std::out_of_range &) {
            this->arguments_.configFile.push_back(argv[i]);
        }
    }
    if (this->arguments_.configFile.empty()) {
        throw ArgumentException("at least one config file is mendatory");
    }
}

const ArgManager::ArgumentStruct &ArgManager::getArguments() const noexcept {
    return this->arguments_;
}
