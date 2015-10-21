#include "BoundingSphereMesh.h"

glm::vec3 BoundingSphereMesh::getNormal(const glm::vec3& position) const {
  return -1.0f * SphereMesh::getNormal(position);
}
