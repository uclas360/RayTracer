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

namespace RayTracer {

void CustomShape::parseVertex(std::vector<std::string> args) {
  if (args.size() != 3) throw std::out_of_range("v: NOT ENOUGH COORDS");
  _vertices.push_back(Math::Vector3D(std::stof(args[0]), std::stof(args[1]),
                                     std::stof(args[2])));
}

void CustomShape::parseTexture(std::vector<std::string> args) {
  if (args.size() != 2) throw std::out_of_range("vt: NOT ENOUGH COORDS");
  _textureVertices.push_back(
      Math::Vector3D(std::stof(args[0]), std::stof(args[1]), 0));
}

void CustomShape::parseNormals(std::vector<std::string> args) {
  if (args.size() != 3) throw std::out_of_range("vn: NOT ENOUGH COORDS");
  _normals.push_back(Math::Vector3D(std::stof(args[0]), std::stof(args[1]),
                                    std::stof(args[2])));
}

void CustomShape::parseFace(std::vector<std::string> args) {
  std::vector<std::string> vectors;
  std::string tmp;
  std::stringstream stream;
  std::vector<Math::Vector3D> points;

  for (std::string vertex : args) {
    stream = std::stringstream(vertex);
    vectors.clear();
    while (std::getline(stream, tmp, '/')) {
      vectors.push_back(tmp);
    }
    points.push_back((_vertices[std::stoi(vectors[0]) - 1]));
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

void CustomShape::parseLine(std::string &line) {
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

CustomShape::CustomShape(const libconfig::Setting &settings) {
  std::string path;

  _triangleLoader = getLoader();
  settings.lookupValue("file", path);
  std::ifstream file(path);
  std::string line;

  while (getline(file, line)) {
    parseLine(line);
  }
}

HitRecord CustomShape::hits(const Ray &ray) const {
  HitRecord record, temp;
  std::vector<IShape> faces;

  for (size_t i = 0; i < _faces.size(); ++i) {
    temp = _faces[i]->hits(ray);
    if ((temp.t != 0 && temp.t <= record.t) || record.t == 0) {
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