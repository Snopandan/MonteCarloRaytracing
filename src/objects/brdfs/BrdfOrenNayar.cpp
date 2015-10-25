#include "BrdfOrenNayar.h"

BrdfOrenNayar::BrdfOrenNayar(float reflectionCoefficient, float deviation)
: reflectionCoefficient_(reflectionCoefficient), deviation_(deviation) {

}

BrdfOrenNayar::~BrdfOrenNayar() {

}

float BrdfOrenNayar::compute(const glm::vec3& position, const glm::vec2& incoming, const glm::vec2& outgoing) const {
  float deviationSquared = std::pow(deviation_, 2);
  float a = 1 - (deviationSquared / (2.0f * (deviationSquared + 0.33f)));
  float b = (0.45f*deviationSquared) / (deviationSquared + 0.09f);

  float alpha = std::max(incoming.x, outgoing.x);
  float beta = std::min(incoming.x, outgoing.x);

  return (reflectionCoefficient_ / M_PI) * (a + b*std::max(0.0f, std::cos(incoming.y - outgoing.y) * std::sin(alpha) * std::sin(beta)));
}
