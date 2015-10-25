#include "OpaqueObject.h"

OpaqueObject::OpaqueObject(const std::string& name, Mesh* mesh, Brdf* brdf, const bool isLight, const glm::vec3& intensity)
: Object(name, mesh, false, isLight), brdf_(brdf), intensity_{intensity} {

}

OpaqueObject::~OpaqueObject() {
  delete brdf_;
}

void OpaqueObject::setIntensity(const glm::vec3& intensity) {
  intensity_ = intensity;
}

void OpaqueObject::addIntensity(const glm::vec3& intensity) {
  intensity_ += intensity;
}

