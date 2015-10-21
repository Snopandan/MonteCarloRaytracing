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

  // std::cout << "R1: " << r1 << std::endl;
  // std::cout << "R2: " << r2 << std::endl;

  float phi = 2 * M_PI * r1;
  float theta = std::acos(std::sqrt(r2));

  return glm::vec2(phi, theta);
}

inline bool shouldTerminateRay(const float phi, const float probabilityNotToTerminateRay) {
  return (phi / probabilityNotToTerminateRay) > (2*M_PI);
}


inline float getEpsilon() {
  return 0.0001f;
}

inline bool equalsEpsilon(const float f1, const float f2) {
  // static float epsilon = 0.00001f;
  // return  f1 == f2;
  return ((f2 - getEpsilon()) <= f1) && (f1 <= (f2 + getEpsilon()));
}


#endif
