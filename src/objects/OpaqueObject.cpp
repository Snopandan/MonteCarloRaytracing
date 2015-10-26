#include "OpaqueObject.h"

OpaqueObject::OpaqueObject(const std::string& name, Mesh* mesh, Brdf* brdf, const bool isLight, const glm::vec3& intensity)
: Object(name, mesh, false, isLight, intensity), brdf_(brdf) {

}

OpaqueObject::~OpaqueObject() {
  delete brdf_;
}
