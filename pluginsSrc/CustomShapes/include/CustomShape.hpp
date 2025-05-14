/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CustomShape
*/

#ifndef CUSTOMSHAPE_HPP_
#define CUSTOMSHAPE_HPP_

#include <functional>
#include <libconfig.h++>
#include <map>

#include "Raytracer/Ray.hpp"
#include "Raytracer/math/Vector.hpp"
#include "plugins/IShape.hpp"
#include "libLoaders/LDLoader.hpp"

namespace RayTracer {

class CustomShape : public IShape {
 public:
  CustomShape(const libconfig::Setting &settings);
  ~CustomShape();
  HitRecord hits(const Ray &) const override;
  void move(const Math::Vector3D &offset) override;
  void rotate(const Math::Vector3D &angles) override;
  void scale(size_t) override;
  void setPosition(const Math::Vector3D &) override;
  void save(libconfig::Setting &parent) const override;

 private:
  void parseLine(const std::string &line);
  void parseVertex(const std::vector<std::string> &args);
  void parseTexture(const std::vector<std::string> &args);
  void parseNormals(const std::vector<std::string> &args);
  void parseFace(const std::vector<std::string> &args);
  void getPos(const libconfig::Setting &settings);
  void getRotation(const libconfig::Setting &settings);
  void getScale(const libconfig::Setting &settings);

  std::vector<Math::Vector3D> vertices_;
  std::vector<Math::Vector3D> textureVertices_;
  std::vector<Math::Vector3D> normals_;

  std::string path_;

    double scale_ = 100;
    Math::Vector3D pos_;
    Math::Vector3D rotation_;

  std::unique_ptr<DlLoader<IShape>> triangleLoader_;

  std::vector<std::unique_ptr<IShape>> faces_;
  const std::map<std::string,
                 std::function<void(CustomShape *, std::vector<std::string>)>>
      functions_ = {
          {"v", &CustomShape::parseVertex},
          {"vt", &CustomShape::parseTexture},
          {"vn", &CustomShape::parseNormals},
          {"f", &CustomShape::parseFace},
      };
};
}  // namespace RayTracer


#endif /* !CUSTOMSHAPE_HPP_ */
