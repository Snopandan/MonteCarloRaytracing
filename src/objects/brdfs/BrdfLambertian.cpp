#include "BrdfLambertian.h"

BrdfLambertian::BrdfLambertian(float reflectionCoefficient)
: reflectionCoefficient_(reflectionCoefficient) {

}

BrdfLambertian::~BrdfLambertian() {

}

float BrdfLambertian::compute(const glm::vec3& position, const glm::vec2& incoming, const glm::vec2& outgoing) const {
  return reflectionCoefficient_ / M_PI;
}
