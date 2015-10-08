#include "Ray.h"

Ray::Ray(const glm::vec3 origin, const glm::vec3 direction)
: origin_{origin}
, direction_{direction}
{

}


glm::vec3 Ray::getOrigin() const {
  return origin_;
}


glm::vec3 Ray::getDirection() const {
  return direction_;
}


bool Ray::triangleIntersection(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3& intersection) {
  glm::vec3 D = direction_;
  glm::vec3 O = origin_;
  
  float det, inv_det, u, v;
  float t;

  // Find vectors for two edges sharing v1
  glm::vec3 e1 = v2 - v1;
  glm::vec3 e2 = v3 - v1;

  // Begin calculating determinant - also used to calculate u parameter
  glm::vec3 P = glm::cross(D, e2);

  // If determinant is near zero, ray lies in plane of triangle
  det = glm::dot(e1, P);

  // NOT CULLING
  if(det > -EPSILON && det < EPSILON) {
    return false;
  }
  inv_det = 1.0f / det;

  // Calculate distance from v1 to ray origin
  glm::vec3 T = O - v1;

  // Calculate u parameter and test bound
  u = glm::dot(T, P) * inv_det;

  // The intersection lies outside of the triangle
  if(u < 0.0f || u > 1.0f) {
    return false;
  }

  // Prepare to test v parameter
  glm::vec3 Q = glm::cross(T, e1);

  // Calculate V parameter and test bound
  v = glm::dot(D, Q) * inv_det;

  // The intersection lies outside of the triangle
  if(v < 0.0f || u + v  > 1.0f) {
    return false;
  }

  t = glm::dot(e2, Q) * inv_det;

  if(t > EPSILON) { //ray intersection
    // *out = t;
    // std::cout << "t: " << t << std::endl;
    intersection = O + t * D;
    // std::cout << "z: " << ray->getOrigin().z + ray->getDirection().z * t << std::endl;
    return true;
  }

  return false;
}




