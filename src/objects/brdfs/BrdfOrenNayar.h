#ifndef BRDF_OREN_NAYAR_H
#define BRDF_OREN_NAYAR_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Brdf.h"

class BrdfOrenNayar : public Brdf{
public:
  BrdfOrenNayar(float reflectionCoefficient, float deviation);
  virtual ~BrdfOrenNayar();

  virtual float compute(const glm::vec3& position, const glm::vec2& incoming, const glm::vec2& outgoing) const override;
protected:

private:
  float reflectionCoefficient_;
  float deviation_;
};
#endif
