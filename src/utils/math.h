#ifndef MATH_H
#define MATH_H

#include <cmath>

#include "Ray.h"

static float clamp(float value, float min, float max) {
  return value < min ? min : (value > max ? max : value);
}

static float cos2Theta(const Ray& ray) {
  glm::vec3 vector = ray.getAsVector();
  return vector.z * vector.z;
}

static float cosTheta(const Ray& ray) {
  glm::vec3 vector = ray.getAsVector();
  return vector.z;
}

static float sin2Theta(const Ray& ray) {
  return std::max(0.0f, 1.0f - cos2Theta(ray));
}
static float sinTheta(const Ray& ray) {
  return std::sqrt(sin2Theta(ray));
}

static float sinPhi(const Ray& ray) {
  glm::vec3 vector = ray.getAsVector();
  float sTheta = sinTheta(ray);
  return (sTheta == 0.0f) ? 0.0f : clamp(vector.y / sTheta, -1.0f, 1.0f);
}

static float sin2Phi(const Ray& ray) {
  return sinPhi(ray) * sinPhi(ray);
}

static float cosPhi(const Ray& ray) {
  glm::vec3 vector = ray.getAsVector();
  float sTheta = sinTheta(ray);
  return (sTheta == 0.0f) ? 1.0f : clamp(vector.x / sTheta, -1.0f, 1.0f);
}

static float cos2Phi(const Ray& ray) {
  return cosPhi(ray) * cosPhi(ray);
}

#endif
