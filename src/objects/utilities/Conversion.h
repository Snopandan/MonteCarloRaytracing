#ifndef CONVERSION_H
#define CONVERSION_H

#include <cmath>

#include "glm/glm.hpp"

#include "Ray.h"

namespace util {
  glm::vec2 rayToAngles(Ray ray, glm::vec3 normal);
}
#endif
