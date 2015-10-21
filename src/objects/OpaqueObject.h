#ifndef OPAQUE_OBJECT_H
#define OPAQUE_OBJECT_H

#include "Object.h"
#include "meshes/Mesh.h"
#include "brdfs/Brdf.h"

class OpaqueObject : public Object {
public:
  OpaqueObject(Mesh* mesh, Brdf* brdf, const bool isLight = false, const glm::vec3& intensity = glm::vec3{0.0f, 0.0f, 0.0f});
  virtual ~OpaqueObject();

  float computeBrdf(glm::vec3 position, glm::vec2 incoming, glm::vec2 outgoing) const { return brdf_->compute(position, incoming, outgoing); }
  glm::vec3 getIntensity() const override { return intensity_; }

  void setIntensity(const glm::vec3& intensity);
  void addIntensity(const glm::vec3& intensity);

private:
  Brdf* brdf_;
  glm::vec3 intensity_;

};
#endif
