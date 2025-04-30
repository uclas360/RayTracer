/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** Camera
*/

#include "raytracer/Camera.hpp"
#include <iostream>
#include <utility>
#include "RaytracerCore.hpp"

RayTracer::Camera::Camera(libconfig::Setting &settings) {
    double posX;
    double posY;
    double posZ;

    if (!settings.lookupValue("posX", posX)) {
        throw ParsingException("error creating camera, posX field missing");
    }
    if (!settings.lookupValue("posY", posY)) {
        throw ParsingException("error creating camera, posY field missing");
    }
    if (!settings.lookupValue("posZ", posZ)) {
        throw ParsingException("error creating camera, posZ field missing");
    }
    this->pos = {posX, posY, posZ};
    std::cout << this->pos << std::endl;
};

RayTracer::Camera::Camera(RayTracer::Camera &&other): pos(std::move(other.pos)) {}
