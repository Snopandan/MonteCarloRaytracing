#ifndef OPAQUE_OBJECT_H
#define OPAQUE_OBJECT_H

#include "Object.h"
#include "meshes/Mesh.h"
#include "brdfs/Brdf.h"

class OpaqueObject : public Object {
public:
  OpaqueObject(const std::string& name, Mesh* mesh, Brdf* brdf, const bool isLight = false, const glm::vec3& intensity = glm::vec3{1.0f, 1.0f, 1.0f});
  virtual ~OpaqueObject();

  float computeBrdf(const glm::vec3& position, const glm::vec2& incoming, const glm::vec2& outgoing) const { return brdf_->compute(position, incoming, outgoing); }

private:
  Brdf* brdf_;

};
#endif
