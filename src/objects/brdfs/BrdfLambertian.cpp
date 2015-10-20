#include "BrdfLambertian.h"

BrdfLambertian::BrdfLambertian(float reflectionCoefficient)
: reflectionCoefficient_(reflectionCoefficient) {

}

BrdfLambertian::~BrdfLambertian() {

}

float BrdfLambertian::compute(glm::vec3 position, glm::vec2 incoming, glm::vec2 outgoing) const {
  return reflectionCoefficient_ / M_PI;
}
