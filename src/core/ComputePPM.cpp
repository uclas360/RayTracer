/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core compute
*/

#include <cstdint>
#include <format>
#include <fstream>
#include <vector>

#include "CustomException.hpp"
#include "RaytracerCore.hpp"

void RaytracerCore::computeOutput() {
    for (auto &it : this->threads_) {
        it.join();
    }
    size_t wScale = this->width_ / this->xResolution_;
    size_t hScale = this->height_ / this->yResolution_;

    std::ofstream file("output.ppm");

    if (!file.is_open()) {
        throw CustomException("failed to create file for ppm output");
    }
    std::string header =
        std::format("P3\n{} {}\n255\n", this->width_, this->height_);
    file.write(header.c_str(), header.length());
    for (size_t y = 0; y < this->height_; y++) {
        for (size_t x = 0; x < this->width_; x++) {
            std::uint8_t *pixels =
                this->imageMean_.data() +
                ((y / hScale) * this->xResolution_ + (x / wScale)) * 4;
            file << (int)pixels[0] << " " << (int)pixels[1] << " " << (int)pixels[2] << "\n";
        }
    }
}
