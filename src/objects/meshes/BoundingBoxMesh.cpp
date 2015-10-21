#include "BoundingBoxMesh.h"

glm::vec3 BoundingBoxMesh::getNormal(const glm::vec3& position) const {
  return -1.0f * BoxMesh::getNormal(position);
}
