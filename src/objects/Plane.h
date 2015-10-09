#ifndef PLANE_H
#define PLANE_H

#include <math.h>
#include <iostream>

#include "glm/glm.hpp"

#include "Ray.h"

class Plane  {

public: 

  Plane(const glm::vec3 upperLeftCorner, 
        const glm::vec3 lowerLeftCorner,
        const glm::vec3 lowerRightCorner);

  bool intersect(Ray* ray, glm::vec3& intersection);

protected:

private:
  glm::vec3 upperLeftCorner_;
  glm::vec3 lowerLeftCorner_;
  glm::vec3 lowerRightCorner_;
  glm::vec3 upperRightCorner_;

  glm::vec3 normal_;

};

#endif // PLANE_H