#include "Plane.h"


Plane::Plane(const glm::vec3 upperLeftCorner, 
             const glm::vec3 lowerLeftCorner,
             const glm::vec3 lowerRightCorner,
             const glm::vec3 upperRightCorner)
: upperLeftCorner_{upperLeftCorner}
, lowerLeftCorner_{lowerLeftCorner}
, lowerRightCorner_{lowerRightCorner}
, upperRightCorner_{upperRightCorner} 
{ 

  glm::vec3 edge1 = glm::normalize(upperLeftCorner_ - lowerLeftCorner_);
  glm::vec3 edge2 = glm::normalize(lowerRightCorner_ - lowerLeftCorner_); 

  normal_ = glm::normalize(glm::cross(edge1, edge2));
}


bool Plane::intersect(Ray* ray) {


  return true;
}

