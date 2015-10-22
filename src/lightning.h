#ifndef LIGHTNING_H
#define LIGHTNING_H

#include "glm/glm.hpp"

#include "utils/math.h"


inline glm::vec3 localLightning(const glm::vec3& origin, 
                                const glm::vec3& direction, 
                                const glm::vec3& normal, 
                                const glm::vec3& viewDirection,
                                const float ka,
                                const float kd,
                                const float ks,
                                const glm::vec3& ambientColor,
                                const glm::vec3& diffuseColor,
                                const glm::vec3& specularColor,
                                const float specularity) {

  const glm::vec3 L = -direction;
  const glm::vec3 N = normal;
  const glm::vec3 V = -viewDirection;
  const glm::vec3 R = glm::reflect(L, N);

  return ka * ambientColor
         + kd * std::max(0.0f, glm::dot(L, N)) * diffuseColor
         + ks * std::pow(clamp(glm::dot(R, V), 0.0f, 1.0f), specularity) * specularColor;

}


#endif // LIGHTNING_H