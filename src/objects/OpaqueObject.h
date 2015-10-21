#ifndef OPAQUE_OBJECT_H
#define OPAQUE_OBJECT_H

#include "Object.h"
#include "meshes/Mesh.h"
#include "brdfs/Brdf.h"

class OpaqueObject : public Object{
public:
  OpaqueObject(Mesh* mesh, Brdf* brdf, const bool isLight = false);
  virtual ~OpaqueObject();

  float computeBrdf(glm::vec3 position, glm::vec2 incoming, glm::vec2 outgoing) const { return brdf_->compute(position, incoming, outgoing); }

private:
  Brdf* brdf_;

};
#endif
