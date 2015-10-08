#include "SphereMesh.h"

SphereMesh::SphereMesh(const glm::vec3 position, const float radius)
: position_{position}
, radius_{radius} 
, radiusPow2_{std::pow(radius, 2)}
{

}

SphereMesh::~SphereMesh() {

}

std::tuple<Mesh::Intersection, float, float> SphereMesh::getIntersections(const Ray* ray) const {
    const glm::vec3 c = position_;
    const glm::vec3 o = ray->getOrigin();
    const glm::vec3 d = ray->getDirection();

    const float denominator = glm::dot(d, d);

    const glm::vec3 oMinusC = o - c;
    const float dDotOMinusC = glm::dot(d, oMinusC);

    const float numeratorFirstPart = -dDotOMinusC;
    const float numeratorSecondPart = std::pow(dDotOMinusC, 2) - denominator * (glm::dot(oMinusC, oMinusC) - radiusPow2_);

    float sMin = 0;
    float sMax = 0;

    if (numeratorSecondPart == 0.0) {
      sMin = numeratorFirstPart / denominator;
      return std::make_tuple(Mesh::Intersection::SINGLE_HIT, sMin, sMax);

    } else if(numeratorSecondPart > 0) {

      const float sqrtNumeratorSecondPart = std::sqrt(numeratorSecondPart);
      const float denominatorInverse = 1.0 / denominator;

      sMin = (numeratorFirstPart - sqrtNumeratorSecondPart) * denominatorInverse;
      sMax = (numeratorFirstPart + sqrtNumeratorSecondPart) * denominatorInverse;

      if (sMax < 0) {
        return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMax, sMin);
      } else {
        return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMin, sMax);
      }

    }

    return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);
}
