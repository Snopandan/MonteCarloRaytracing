#include "Ray.h"

Ray::Ray(const glm::vec3 origin, const glm::vec3 direction)
: origin_{origin}
, direction_{direction}
, inversedDirection_{1.0 / direction.x, 1.0 / direction.y, 1.0 / direction.z}
{

}


glm::vec3 Ray::getOrigin() const {
  return origin_;
}


glm::vec3 Ray::getDirection() const {
  return direction_;
}


glm::vec3 Ray::getInversedDirection() const {
  return inversedDirection_;
}


glm::vec2 Ray::getAngles() const {
  glm::vec3 vec = origin_ + direction_;

  const float r = glm::length(vec);
  const float inclination = std::acos(vec.z / r);
  const float horizontal = std::atan2(vec.y, vec.x);

  return glm::vec2{inclination, horizontal};
}


