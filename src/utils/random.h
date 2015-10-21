#ifndef RANDOM_H
#define RANDOM_H

#include <iostream>
#include <random>
#include <cmath>

#include "glm/glm.hpp"

inline float random0To1() {
  static std::default_random_engine generator;
  static std::uniform_real_distribution<float> unifrom0To1Distribution(0.0, 1.0);
  return unifrom0To1Distribution(generator);
}

inline glm::vec2 getRandomAngles() {
  float r1 = random0To1();
  float r2 = random0To1();

  float phi = 2 * M_PI * r1;
  float theta = std::acos(std::sqrt(r2));

  return glm::vec2(phi, theta);
}

inline bool shouldTerminateRay(const float phi, const float probabilityNotToTerminateRay) {
  return (phi / probabilityNotToTerminateRay) > (2*M_PI);
}



#endif
