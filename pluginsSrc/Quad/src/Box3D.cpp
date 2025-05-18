/*
** EPITECH PROJECT, 2025
** Box
** File description:
** Box
*/

#include "../include/Box3D.hpp"

#include <algorithm>
#include <libconfig.h++>

#include "RaytracerCore.hpp"
#include "plugins/IShape.hpp"

namespace RayTracer {
Box3D::Box3D(const libconfig::Setting &settings) {
    try {
        libconfig::Setting &tip = settings.lookup("tmin");
        if (!Math::lookUpVector(tip, this->_tmin)) {
            throw ParsingException(
                "error parsing quad object, missing \"tmin\" field");
        }
    } catch (const libconfig::SettingNotFoundException &) {
        throw ParsingException(
            "error parsing quad object, missing \"tmin\" field");
    }
    try {
        libconfig::Setting &tip = settings.lookup("tmax");
        if (!Math::lookUpVector(tip, this->_tmax)) {
            throw ParsingException(
                "error parsing quad object, missing \"tmax\" field");
        }
    } catch (const libconfig::SettingNotFoundException &) {
        throw ParsingException(
            "error parsing quad object, missing \"tmax\" field");
    }
    this->bbox = {this->_tmin, this->_tmax};
}

HitRecord Box3D::hits(const Ray &ray, Interval) const {
    double tmin = ((_tmin.x - ray.pos.x) / ray.dir.x);
    double tmax = ((_tmax.x - ray.pos.x) / ray.dir.x);
    double tymin = ((_tmin.y - ray.pos.y) / ray.dir.y);
    double tymax = ((_tmax.y - ray.pos.y) / ray.dir.y);
    double tzmin = ((_tmin.z - ray.pos.z) / ray.dir.z);
    double tzmax = ((_tmax.z - ray.pos.z) / ray.dir.z);

    if (ray.dir.x == 0) {
        if (ray.pos.x < _tmin.x || ray.pos.x > _tmax.x) return HitRecord();
    } else if (tmin > tmax) {
        std::swap(tmin, tmax);
    }
    if (ray.dir.y == 0) {
        if (ray.pos.y < _tmin.y || ray.pos.y > _tmax.y) return HitRecord();
    } else if (tymin > tymax) {
        std::swap(tymin, tymax);
    }

    if ((tmin > tymax) || (tymin > tmax)) return HitRecord();

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    if (ray.dir.z == 0) {
        if (ray.pos.z < _tmin.z || ray.pos.z > _tmax.z) return HitRecord();
    } else if (tzmin > tzmax) {
        std::swap(tzmin, tzmax);
    }

    if ((tmin > tzmax) || (tzmin > tmax)) return HitRecord();

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    double t_hit = tmin;
    if (tmin < 0) t_hit = tmax;
    if (t_hit < 0) return HitRecord();

    Math::Vector3D hit_point = ray.at(t_hit);

    Math::Vector3D normal;
    const double epsilon = 1e-6;

    if (std::abs(hit_point.x - _tmin.x) < epsilon)
        normal = {-1, 0, 0};
    else if (std::abs(hit_point.x - _tmax.x) < epsilon)
        normal = {1, 0, 0};
    else if (std::abs(hit_point.y - _tmin.y) < epsilon)
        normal = {0, -1, 0};
    else if (std::abs(hit_point.y - _tmax.y) < epsilon)
        normal = {0, 1, 0};
    else if (std::abs(hit_point.z - _tmin.z) < epsilon)
        normal = {0, 0, -1};
    else if (std::abs(hit_point.z - _tmax.z) < epsilon)
        normal = {0, 0, 1};

    return HitRecord(t_hit, ray, *this, normal, this->material_);
}

void Box3D::move(const Math::Vector3D &pos) {
    _tmin += pos;
    _tmax += pos;
}

void Box3D::rotate(const Math::Vector3D &angles) {
    _tmin.rotateX(angles.x);
    _tmax.rotateX(angles.x);

    _tmin.rotateY(angles.y);
    _tmax.rotateY(angles.y);

    _tmin.rotateZ(angles.z);
    _tmax.rotateZ(angles.z);
}

void Box3D::scale(size_t scale) {
    Math::Vector3D center = (_tmin + _tmax) / 2.0;

    _tmin = center + (_tmin - center) * (double)scale / 100;
    _tmax = center + (_tmax - center) * (double)scale / 100;
}

void Box3D::setPosition(const Math::Vector3D &newPos) {
    Math::Vector3D currentCenter = (_tmin + _tmax) / 2.0;
    Math::Vector3D offset = newPos - currentCenter;

    move(offset);
}
}
