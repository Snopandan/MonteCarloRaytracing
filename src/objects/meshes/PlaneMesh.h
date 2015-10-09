#ifndef PLANEMESH_H
#define PLANEMESH_H

#include <math.h>
#include <iostream>

#include "glm/glm.hpp"

#include "TriangleMesh.h"


class PlaneMesh : public TriangleMesh {

public: 

  PlaneMesh(const glm::vec3 upperLeftCorner, 
            const glm::vec3 lowerLeftCorner,
            const glm::vec3 lowerRightCorner);

protected:

private:
  glm::vec3 upperLeftCorner_;
  glm::vec3 lowerLeftCorner_;
  glm::vec3 lowerRightCorner_;
  glm::vec3 upperRightCorner_;

  glm::vec3 normal_;

};


#endif // PLANEMESH_H