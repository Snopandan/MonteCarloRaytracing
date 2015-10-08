#include "Plane.h"


Plane::Plane(const glm::vec3 upperLeftCorner, 
             const glm::vec3 lowerLeftCorner,
             const glm::vec3 lowerRightCorner)
: upperLeftCorner_{upperLeftCorner}
, lowerLeftCorner_{lowerLeftCorner}
, lowerRightCorner_{lowerRightCorner}
{ 

  glm::vec3 edge1 = upperLeftCorner_ - lowerLeftCorner_;
  glm::vec3 edge2 = lowerRightCorner_ - lowerLeftCorner_; 

  normal_ = glm::normalize(glm::cross(edge1, edge2));

  upperRightCorner_ = lowerRightCorner_ + edge1;

}



#define EPSILON 0.000001

bool Plane::intersect(Ray* ray, glm::vec3& intersection) {
  // bool result = ray->triangleIntersection(upperLeftCorner_, lowerLeftCorner_, lowerRightCorner_, intersection) ||
  //               ray->triangleIntersection(lowerRightCorner_, upperRightCorner_, upperLeftCorner_, intersection);

  bool result = false;
  return result;
}


