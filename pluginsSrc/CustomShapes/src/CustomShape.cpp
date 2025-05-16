/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CustomShape
*/

#include "CustomShape.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "RaytracerCore.hpp"

namespace RayTracer {

void CustomShape::parseVertex(const std::vector<std::string> &args) {
  if (args.size() != 3) throw ParsingException("v: NOT ENOUGH COORDS");
  try {
    this->vertices_.push_back(Math::Vector3D(std::stof(args[0]), std::stof(args[1]),
                                       std::stof(args[2])) * scale_);
  } catch (const std::invalid_argument &e) {
    throw ParsingException("Error parsing custom shape vertex, wrong double");
  }
}

void CustomShape::parseTexture(const std::vector<std::string> &args) {
  if (args.size() != 2) throw ParsingException("vt: NOT ENOUGH COORDS");
  try {
    this->textureVertices_.push_back(
        Math::Vector3D(std::stof(args[0]), std::stof(args[1]), 0));
  } catch (const std::invalid_argument &e) {
    throw ParsingException(
        "Error parsing custom shape texture vertex, wrong double");
  }
}

void CustomShape::parseNormals(const std::vector<std::string> &args) {
  if (args.size() != 3) throw ParsingException("vn: NOT ENOUGH COORDS");
  try {
    this->normals_.push_back(Math::Vector3D(std::stof(args[0]), std::stof(args[1]),
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
  std::vector<Math::Vector3D> textures;
  
  for (std::string vertex : args) {
    stream = std::stringstream(vertex);
    vectors.clear();
    while (std::getline(stream, tmp, '/')) {
      vectors.push_back(tmp);
    }
    points.push_back((vertices_[std::stoi(vectors[0]) - 1]));
    textures.push_back((textureVertices_[std::stoi(vectors[1]) - 1]));
    if (vectors.size() == 3)
      normals.push_back((normals_[std::stoi(vectors[2]) - 1]));
  }
  faces_.push_back(
      triangleLoader_
          ->getInstance<Math::Vector3D, Math::Vector3D, Math::Vector3D>(
              "value_entry_point", points[0], points[1], points[2]));
  if (points.size() == 4) {
    this->faces_.push_back(
        this->triangleLoader_
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
  if (this->functions_.find(type) != this->functions_.end())
    this->functions_.at(type)(this, args);
}

static std::unique_ptr<DlLoader<IShape>> getLoader(void) {
#if defined __linux__
  return std::make_unique<DlLoader<IShape>>("plugin/triangle");
#endif
}

void CustomShape::rotate(const Math::Vector3D &angles) {
  //Math::Vector3D toOrigin = -pos_;

  for (size_t i = 0; i < this->faces_.size(); ++i) {
    this->faces_[i]->rotate(angles);
  }
  this->rotation_ += angles;
}

void CustomShape::setPosition(const Math::Vector3D &pos) {
  Math::Vector3D toPos = pos - this->pos_;
  this->move(toPos);
  this->pos_ = pos;
}

void CustomShape::move(const Math::Vector3D &offset) {
  for (size_t i = 0; i < this->faces_.size(); ++i) {
    this->faces_[i]->move(offset);
  }
  this->pos_ += offset;
}

void CustomShape::getPos(const libconfig::Setting &settings) {
  try {
    libconfig::Setting &pos = settings.lookup("pos");
    if (!Math::lookUpVector(pos, this->pos_)) {
      throw ParsingException(
          "error parsing custom shape object, wrong \"pos\" field");
    }
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
  setPosition(pos_);
}

void CustomShape::getRotation(const libconfig::Setting &settings) {
  Math::Vector3D rotation;
  try {
    libconfig::Setting &pos = settings.lookup("rotation");
    if (!Math::lookUpVector(pos, rotation)) {
      throw ParsingException(
          "error parsing custom shape object, wrong \"rotation\" field");
    }
  } catch (const ParsingException &e) {
    throw e;
  } catch (const libconfig::SettingNotFoundException &e) {
    throw ParsingException(e.what());
  }
  rotate(rotation);
}

void CustomShape::scale(size_t) {
  return;
  // Math::Vector3D pos = this->pos_;
  // for (size_t i = 0; i < faces_.size(); ++i) {
  //   faces_[i].release();
  // }
  // faces_.clear();
  // this->normals_.clear();

  // this->setPosition(Math::Vector3D());
  // for (size_t i = 0; i < vertices_.size(); ++i) {
  //   vertices_[i] *= (scale / 100);
  // }
  // for (const std::vector<std::string> &args : facesLines_) {
  //   this->parseFace(args);
  // }
  // this->setPosition(pos);
}

void CustomShape::getScale(const libconfig::Setting &settings) {
  double newscale = 0;

  if (!settings.lookupValue("scale", newscale)) {
    return;
  }
  scale_ = newscale;
  // scale(scale_);
}

CustomShape::CustomShape(const libconfig::Setting &settings) {
  this->triangleLoader_ = getLoader();
  if (!settings.lookupValue("file", this->path_))
    throw ParsingException(
        "Error parsing custom shape, missing \"file\" field");
  std::ifstream file(this->path_);
  std::string line;

  if (!file.is_open()) {
    throw ParsingException("error parsing custom shape, file not openned");
  }
  getScale(settings);
  while (getline(file, line)) {
    parseLine(line);
  }
  file.close();
  getPos(settings);
  getRotation(settings);
}

HitRecord CustomShape::hits(const Ray &ray) const {
  HitRecord record;
  double closest_t = INFINITY;

  for (const auto &face : this->faces_) {
    HitRecord temp = face->hits(ray);
    if (!temp.missed && temp.t > 0 && temp.t < closest_t) {
      closest_t = temp.t;
      record = temp;
    }
  }
  return record;
}

void CustomShape::save(libconfig::Setting &parent) const {
  libconfig::Setting &shapeSettings = parent.add(libconfig::Setting::TypeGroup);
  shapeSettings.add("type", libconfig::Setting::TypeString) = "shape";
  shapeSettings.add("name", libconfig::Setting::TypeString) = "customShape";
  libconfig::Setting &data =
      shapeSettings.add("data", libconfig::Setting::TypeGroup);
  data.add("file", libconfig::Setting::TypeString) = this->path_;
  libconfig::Setting &pos = data.add("pos", libconfig::Setting::TypeGroup);
  Math::writeUpVector(pos, this->pos_);
  libconfig::Setting &rotation =
      data.add("rotation", libconfig::Setting::TypeGroup);
  Math::writeUpVector(rotation, this->rotation_);
}

CustomShape::~CustomShape() {}

extern "C" {
void *entry_point(const libconfig::Setting &config) {
  return new RayTracer::CustomShape(config);
}
}
}  // namespace RayTracer
