/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CustomShape
*/

#ifndef CUSTOMSHAPE_HPP_
#define CUSTOMSHAPE_HPP_

#include <fstream>
#include <functional>
#include <iostream>
#include <libconfig.h++>
#include <map>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/AShape.hpp"
#include "libLoaders/LDLoader.hpp"

namespace RayTracer {

class CustomShape : public AShape {
 public:
  CustomShape(const libconfig::Setting &settings);
  ~CustomShape();
  HitRecord hits(const Ray &, Interval ray_t) const override;
  void move(const Math::Vector3D &) override {};
  void rotate(const Math::Vector3D &angles) override;
  void scale(size_t) override;
  void setPosition(const Math::Vector3D &) override;

 private:
  void parseLine(const std::string &line);
  void parseVertex(const std::vector<std::string> &args);
  void parseTexture(const std::vector<std::string> &args);
  void parseNormals(const std::vector<std::string> &args);
  void parseFace(const std::vector<std::string> &args);
  void getPos(const libconfig::Setting &settings);
  void getRotation(const libconfig::Setting &settings);
  void getScale(const libconfig::Setting &settings);
  std::vector<Math::Vector3D> _vertices;
  std::vector<Math::Vector3D> _textureVertices;
  std::vector<Math::Vector3D> _normals;

    double scale_ = 100;
    Math::Vector3D pos_;
    Math::Vector3D rotation_;

  std::unique_ptr<DlLoader<IShape>> _triangleLoader;

  std::vector<std::unique_ptr<IShape>> _faces;
  const std::map<std::string,
                 std::function<void(CustomShape *, std::vector<std::string>)>>
      _functions = {
          {"v", &CustomShape::parseVertex},
          {"vt", &CustomShape::parseTexture},
          {"vn", &CustomShape::parseNormals},
          {"f", &CustomShape::parseFace},
      };
};
}  // namespace RayTracer


#endif /* !CUSTOMSHAPE_HPP_ */
