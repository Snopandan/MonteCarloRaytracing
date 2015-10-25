#include "BoundingBoxMesh.h"

glm::vec3 BoundingBoxMesh::getNormal(const glm::vec3& position) const {
  return -1.0f * BoxMesh::getNormal(position);
}


glm::vec3 BoundingBoxMesh::getColor(const glm::vec3& position) const {

  if( equalsEpsilon(position.z, zLimits_.y) ) {
    return glm::vec3(0.5f, 0.8f, 0.5f);
  }

  if( equalsEpsilon(position.x, xLimits_.x) ) {
    return glm::vec3(0.1f, 0.12f, 1.0f);
  }

  if( equalsEpsilon(position.x, xLimits_.y) ) {
    return glm::vec3(1.0f, 0.1f, 0.12f);
  }

  if( equalsEpsilon(position.y, yLimits_.x) ) {
    glm::vec2 len{zLimits_.y - zLimits_.x, xLimits_.y - xLimits_.x};
    glm::vec2 uv{(position.z / len.x), (position.x / len.y)};
    const glm::vec3 color1 = glm::vec3{0.9f, 0.9, 0.9f};
    const glm::vec3 color2 = glm::vec3{0.1f, 0.1f, 0.1f};
    return checker(position, color1, color2);
  }

  if( equalsEpsilon(position.y, yLimits_.y) ) {
    return glm::vec3(1.0f, 0.968627451f, 0.8f);
  }


  return glm::vec3(1.0f, 1.0f, 0.1f);

}
