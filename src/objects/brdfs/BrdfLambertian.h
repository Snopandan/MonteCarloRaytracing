#ifndef BRDF_LAMBERTIAN_H
#define BRDF_LAMBERTIAN_H

#define _USE_MATH_DEFINES
#include <cmath>

#include "Brdf.h"

class BrdfLambertian : public Brdf {
public:
  BrdfLambertian(float reflectionCoefficient);
  virtual ~BrdfLambertian();

  virtual float compute(const glm::vec3& position, const glm::vec2& incoming, const glm::vec2& outgoing) const override;

protected:

private:
  float reflectionCoefficient_;
};
#endif
