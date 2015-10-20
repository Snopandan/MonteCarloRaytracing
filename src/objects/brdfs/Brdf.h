#ifndef BRDF_H
#define BRDF_H

#include "glm/glm.hpp"

#include "Ray.h"

class Brdf {
public:
  Brdf();
  virtual ~Brdf();

  virtual float compute(glm::vec3 position, glm::vec2 incoming, glm::vec2 outgoing) const = 0;

private:

};

#endif
