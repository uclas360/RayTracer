/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CustomShape
*/

#include "CustomShape.hpp"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include "RaytracerCore.hpp"

namespace RayTracer {

void CustomShape::parseVertex(const std::vector<std::string> &args) {
  if (args.size() != 3) throw ParsingException("v: NOT ENOUGH COORDS");
  try {
    _vertices.push_back(Math::Vector3D(std::stof(args[0]), std::stof(args[1]),
                                       std::stof(args[2])));
  } catch (const std::invalid_argument &e) {
    throw ParsingException(
        "Error parsing custom shape vertex, wrong double");
  }
}

void CustomShape::parseTexture(const std::vector<std::string> &args) {
  if (args.size() != 2) throw ParsingException("vt: NOT ENOUGH COORDS");
  try {
    _textureVertices.push_back(
        Math::Vector3D(std::stof(args[0]), std::stof(args[1]), 0));
  } catch (const std::invalid_argument &e) {
    throw ParsingException(
        "Error parsing custom shape texture vertex, wrong double");
  }
}

void CustomShape::parseNormals(const std::vector<std::string> &args) {
  if (args.size() != 3) throw ParsingException("vn: NOT ENOUGH COORDS");
  try {
    _normals.push_back(Math::Vector3D(std::stof(args[0]), std::stof(args[1]),
                                      std::stof(args[2])));
  } catch (const std::invalid_argument &e) {
    throw ParsingException(
        "Error parsing custom shape normal vector, wrong double");
  }
}

void CustomShape::parseFace(const std::vector<std::string> &args) {
  std::vector<std::string> vectors;
  std::string tmp;
  std::stringstream stream;
  std::vector<Math::Vector3D> points;
  std::vector<Math::Vector3D> normals;

  for (std::string vertex : args) {
    stream = std::stringstream(vertex);
    vectors.clear();
    while (std::getline(stream, tmp, '/')) {
      vectors.push_back(tmp);
    }
    points.push_back((_vertices[std::stoi(vectors[0]) - 1]));
    if (vectors.size() == 3)
      normals.push_back((_normals[std::stoi(vectors[2]) - 1]));
  }
  _faces.push_back(
      _triangleLoader
          ->getInstance<Math::Vector3D, Math::Vector3D, Math::Vector3D>(
              "value_entry_point", points[0], points[1], points[2]));
  if (points.size() == 4) {
    _faces.push_back(
        _triangleLoader
            ->getInstance<Math::Vector3D, Math::Vector3D, Math::Vector3D>(
                "value_entry_point", points[0], points[2], points[3]));
  }
}

void CustomShape::parseLine(const std::string &line) {
  std::stringstream stream(line.substr(0, line.find("#")));
  std::vector<std::string> args;
  std::string type;
  std::string temp;

  stream >> type;
  while (stream >> temp) args.push_back(temp);
  if (_functions.find(type) != _functions.end())
    _functions.at(type)(this, args);
}

static std::unique_ptr<DlLoader<IShape>> getLoader(void) {
#if defined __linux__
  return std::make_unique<DlLoader<IShape>>("plugin/triangle");
#endif
}

void CustomShape::rotate(const Math::Vector3D &angles) {
  Math::Vector3D toOrigin = -pos_;

  for (size_t i = 0; i < _faces.size(); ++i) {
    _faces[i]->move(toOrigin);
  }
  for (size_t i = 0; i < _faces.size(); ++i) {
    _faces[i]->rotate(angles);
  }
  for (size_t i = 0; i < _faces.size(); ++i) {
    _faces[i]->move((toOrigin * -1));
  }
}

void CustomShape::setPosition(const Math::Vector3D &pos) {
  for (size_t i = 0; i < _faces.size(); ++i) {
    _faces[i]->move(pos);
  }
}

void CustomShape::getPos(const libconfig::Setting &settings) {
  double posX, posY, posZ = 0;

  if (!settings.lookupValue("posX", posX)) {
    throw ParsingException(
        "error parsing custom shape, missing \"posX\" field");
  }
  if (!settings.lookupValue("posY", posY)) {
    throw ParsingException(
        "error parsing custom shape, missing \"posY\" field");
  }
  if (!settings.lookupValue("posZ", posZ)) {
    throw ParsingException(
        "error parsing custom shape, missing \"posZ\" field");
  }
  pos_ = {posX, posY, posZ};
  setPosition(pos_);
}

void CustomShape::getRotation(const libconfig::Setting &settings) {
  double rotationX, rotationY, rotationZ = 0;

  if (!settings.lookupValue("rotationX", rotationX)) {
    throw ParsingException(
        "error parsing custom shape, missing \"rotationX\" field");
  }
  if (!settings.lookupValue("rotationY", rotationY)) {
    throw ParsingException(
        "error parsing custom shape, missing \"rotationY\" field");
  }
  if (!settings.lookupValue("rotationZ", rotationZ)) {
    throw ParsingException(
        "error parsing custom shape, missing \"rotationZ\" field");
  }
  rotation_ = {rotationX, rotationY, rotationZ};
  rotate(rotation_);
}

void CustomShape::scale(size_t) {}

void CustomShape::getScale(const libconfig::Setting &settings) {
  double newscale = 0;

  if (!settings.lookupValue("scale", newscale)) {
    throw ParsingException(
        "error parsing custom shape, missing \"scale\" field");
  }
  scale_ = newscale;
  scale(scale_);
}

CustomShape::CustomShape(const libconfig::Setting &settings) {
  std::string path;

  _triangleLoader = getLoader();
  if (!settings.lookupValue("file", path))
    throw ParsingException(
        "Error parsing custom shape, missing \"file\" field");
  std::ifstream file(path);
  std::string line;

  while (getline(file, line)) {
    parseLine(line);
  }
  getPos(settings);
  getRotation(settings);
}

HitRecord CustomShape::hits(const Ray &ray) const {
  HitRecord record;
  double closest_t = INFINITY;

  for (const auto& face : _faces) {
    HitRecord temp = face->hits(ray);
    if (!temp.missed && temp.t > 0 && temp.t < closest_t) {
      closest_t = temp.t;
      record = temp;
    }
  }
  return record;
}

CustomShape::~CustomShape() {}

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::CustomShape(config);
}
}
}  // namespace RayTracer