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

inline glm::mat3 computeRotationMatrix(const glm::vec3& normal) {
  int imin = 0;
  for(int i=0; i<3; ++i) {
    if(std::abs(normal[i]) < std::abs(normal[imin])) {
      imin = i;
    }
  }
      
  glm::vec3 v2{0.0f, 0.0f, 0.0f};

  const float dt = normal[imin];

  v2[imin] = 1.0f;
  for(int i=0;i<3;i++) {
    v2[i] -= dt*normal[i];
  }

  const glm::vec3 v3 = glm::cross(normal, v2);

  glm::mat3 rotation;
  rotation[0][0] = v2.x;     rotation[0][1] = v2.y;     rotation[0][2] = v2.z;     
  rotation[1][0] = v3.x;     rotation[1][1] = v3.y;     rotation[1][2] = v3.z;     
  rotation[2][0] = normal.x; rotation[2][1] = normal.y; rotation[2][2] = normal.z; 

  return rotation;
}


#endif
