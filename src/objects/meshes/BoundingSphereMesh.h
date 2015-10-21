#ifndef BOUNDING_SPHERE_MESH_H
#define BOUNDING_SPHERE_MESH_H

#include "SphereMesh.h"

class BoundingSphereMesh : public SphereMesh{
public:
  using SphereMesh::SphereMesh;

  virtual glm::vec3 getNormal(const glm::vec3& position) const override;

protected:

private:
};
#endif
