#include "Ray.h"

Ray::Ray(const glm::vec3 origin, const glm::vec3 direction)
: origin_{origin}
, direction_{direction}
{

}


glm::vec3 Ray::getOrigin() const {
  return origin_;
}


glm::vec3 Ray::getDirection() const {
  return direction_;
}


