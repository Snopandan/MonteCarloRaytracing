#ifndef MATH_H
#define MATH_H

#include <cmath>

#include "Ray.h"

float clamp(float value, float min, float max) {
  return value < min ? min : (value > max ? max : value);
}

float cos2Theta(const Ray& ray) {
  glm::vec3 vector = ray.getDirection();
  return vector.z * vector.z;
}

float cosTheta(const Ray& ray) {
  glm::vec3 vector = ray.getDirection();
  return vector.z;
}

float sin2Theta(const Ray& ray) {
  return std::max(0.0f, 1.0f - cos2Theta(ray));
}
float sinTheta(const Ray& ray) {
  return std::sqrt(sin2Theta(ray));
}

float sinPhi(const Ray& ray) {
  glm::vec3 vector = ray.getDirection();
  float sinTheta = SinTheta(ray);
  return (sinTheta == 0.0f) ? 0.0f : clamp(vector.y / sinTheta, -1.0f, 1.0f);
}

float sin2Phi(const Ray& ray) {
  return sinPhi(ray) * sinPhi(ray);
}

float cos2Phi(const Ray& ray) {
  return cosPhi(ray) * cosPhi(ray);
}

float cosPhi(const Ray& ray) {
  glm::vec3 vector = ray.getDirection();
  float sinTheta = sinTheta(ray);
  return (sinTheta == 0.0f) ? 1.0f : clamp(vector.x / sinTheta, -1.0f, 1.0f);
}

#endif
