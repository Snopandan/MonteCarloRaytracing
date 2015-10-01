#include "Plane.h"


Plane::Plane(const glm::vec3 upperLeftCorner, 
             const glm::vec3 lowerLeftCorner,
             const glm::vec3 lowerRightCorner)
: upperLeftCorner_{upperLeftCorner}
, lowerLeftCorner_{lowerLeftCorner}
, lowerRightCorner_{lowerRightCorner}
{ 

  glm::vec3 edge1 = glm::normalize(upperLeftCorner_ - lowerLeftCorner_);
  glm::vec3 edge2 = glm::normalize(lowerRightCorner_ - lowerLeftCorner_); 

  normal_ = glm::normalize(glm::cross(edge1, edge2));

}



#define EPSILON 0.000001

bool Plane::intersect(Ray* ray) {
  glm::vec3 V1 = upperLeftCorner_;
  glm::vec3 V2 = lowerLeftCorner_;
  glm::vec3 V3 = lowerRightCorner_;
  glm::vec3 D = ray->getDirection();
  glm::vec3 O = ray->getOrigin();
  
  float det, inv_det, u, v;
  float t;

  // Find vectors for two edges sharing V1
  glm::vec3 e1 = V2 - V1;
  glm::vec3 e2 = V3 - V1;

  // Begin calculating determinant - also used to calculate u parameter
  glm::vec3 P = glm::cross(D, e2);

  // If determinant is near zero, ray lies in plane of triangle
  det = glm::dot(e1, P);

  // NOT CULLING
  if(det > -EPSILON && det < EPSILON) {
    return false;
  }
  inv_det = 1.0f / det;

  // Calculate distance from V1 to ray origin
  glm::vec3 T = O - V1;

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
    return true;
  }

  return false;
}

