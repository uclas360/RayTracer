/*
** EPITECH PROJECT, 2025
** B-OOP-400-PAR-4-1-raytracer-boris.cheng
** File description:
** material
*/

#include "Raytracer/math/Vector.hpp"

Math::Vector3D reflect(const Math::Vector3D &v,
                              const Math::Vector3D &n) {
    return v - n * (2 * v.dot(n));
}

Math::Vector3D refract(const Math::Vector3D &uv, const Math::Vector3D &n,
                              double etai_over_etat) {
    double cos_theta = std::fmin((-uv).dot(n), 1.0);
    Math::Vector3D r_out_perp = (uv + n * cos_theta) * etai_over_etat;
    Math::Vector3D r_out_parallel =
        n * -std::sqrt(std::fabs(1.0 - r_out_perp.lengthSquared()));
    return r_out_perp + r_out_parallel;
}

double reflectance(double cosine, double refraction_index) {
    double r0 = (1 - refraction_index) / (1 + refraction_index);

    r0 = r0*r0;
    return r0 + (1-r0)*std::pow((1 - cosine),5);
}
