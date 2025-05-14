/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Texture
*/

#include <cstdint>
#include <string>
#include <vector>
#include "Raytracer/math/Vector.hpp"
namespace RayTracer {

class Texture {
    public:
    Texture(std::string filePath);
    Math::Vector3D getColor(double u, double v);

    struct TextureInfos {
        size_t width;
        size_t height;
        size_t maxValue;
    };
    private:
    std::vector<std::vector<Math::Vector3D>> image_;
};

};
