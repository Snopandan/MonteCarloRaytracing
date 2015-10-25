#ifndef BRDF_H
#define BRDF_H

#include "glm/glm.hpp"

#include "Ray.h"

class Brdf {
public:
  Brdf();
  virtual ~Brdf();

  virtual float compute(const glm::vec3& position, const glm::vec2& incoming, const glm::vec2& outgoing) const = 0;

private:

};

#endif
