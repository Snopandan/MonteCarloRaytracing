#include "PlaneMesh.h"


PlaneMesh::PlaneMesh(const glm::vec3 upperLeftCorner, 
                     const glm::vec3 lowerLeftCorner,
                     const glm::vec3 lowerRightCorner)
: TriangleMesh{ std::vector<glm::vec3>{ upperLeftCorner, lowerLeftCorner, lowerRightCorner,
                                        lowerRightCorner, lowerRightCorner + upperLeftCorner - lowerLeftCorner, upperLeftCorner} }
, upperLeftCorner_{upperLeftCorner}
, lowerLeftCorner_{lowerLeftCorner}
, lowerRightCorner_{lowerRightCorner}
, upperRightCorner_{lowerRightCorner + upperLeftCorner - lowerLeftCorner}
{ 

  glm::vec3 edge1 = upperLeftCorner_ - lowerLeftCorner_;
  glm::vec3 edge2 = lowerRightCorner_ - lowerLeftCorner_; 

  normal_ = glm::normalize(glm::cross(edge1, edge2));

}

