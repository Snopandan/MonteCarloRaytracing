#ifndef RANDOM_H
#define RANDOM_H

#include <iostream>
#include <random>
#include <cmath>
#include <stdexcept>

#include "exception/Error.h"

#include "glm/glm.hpp"

inline float random0To1() {
  static std::default_random_engine generator;
  static std::uniform_real_distribution<float> unifrom0To1Distribution(0.0f, 1.0f);
  return unifrom0To1Distribution(generator);
}

inline glm::vec2 getRandomAngles() {
  const float r1 = random0To1();
  const float r2 = random0To1();

  // std::cout << "R1: " << r1 << std::endl;
  // std::cout << "R2: " << r2 << std::endl;

  const float inc = std::acos(std::sqrt(r2));
  const float horizontal = 2.0f * M_PI * r1;

  // std::cout << "horizontal: " << horizontal << std::endl;
  // std::cout << "inc: " << inc << std::endl;


  // if( !((horizontal >= 0.0f) && (horizontal <= (2.0f * M_PI))) ) {
  //   throw std::invalid_argument{ report_error("horizontal angle not correct: " << horizontal << " " << inc) };
  // }

  // if( !((inc >= 0.0f) && (inc <= (M_PI / 2.0f))) ) {
  //   throw std::invalid_argument{ report_error("inclination angle not correct: " << horizontal << " " << inc) };
  // }

  return glm::vec2(inc, horizontal);
}

inline glm::vec3 getRandomVector() {
  const float r1 = random0To1();
  const float r2 = random0To1();

  const float r = std::sqrt(r1);
  const float theta = 2.0f * M_PI * r2;

  const float x = r * std::cos(theta);
  const float y = r * std::sin(theta);

  return glm::vec3{x, y, std::sqrt(std::max(0.0f, 1.0f - r1))};
}

inline bool shouldTerminateRay(const float phi, const float probabilityNotToTerminateRay) {
  return (phi / probabilityNotToTerminateRay) > (2.0f * M_PI);
  // return phi > (2.0f * M_PI * probabilityNotToTerminateRay) ;

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
