/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** argManager
*/

#include "ArgsManager.hpp"

#include <stdexcept>
#include <iostream>
#include <thread>

ArgManager::ArgManager(char **argv) {
    for (int i = 1; argv[i]; i++) {
        try {
            if (this->flagMap.at(argv[i])(*this, argv[i + 1])) argv++;
        } catch (const std::out_of_range &) {
            this->arguments_.configFile.push_back(argv[i]);
        }
    }
    if (this->arguments_.nb_thread == -1) {
        this->arguments_.nb_thread = std::thread::hardware_concurrency();
    }
    if (this->arguments_.height == -1) {
        this->arguments_.height = 800;
    }
    if (this->arguments_.width == -1) {
        this->arguments_.width = 800;
    }
    if (this->arguments_.xResolution == -1 ||
        this->arguments_.xResolution > this->arguments_.width) {
        this->arguments_.xResolution = this->arguments_.width;
    }
    if (this->arguments_.yResolution == -1 ||
        this->arguments_.yResolution > this->arguments_.height) {
        this->arguments_.yResolution = this->arguments_.height;
    }
    if (this->arguments_.finalNbImages == -1) {
        this->arguments_.finalNbImages = 20;
    }
    if (this->arguments_.configFile.empty()) {
        throw ArgumentException("at least one config file is mendatory");
    }
}

const ArgManager::ArgumentStruct &ArgManager::getArguments() const noexcept {
    return this->arguments_;
}
