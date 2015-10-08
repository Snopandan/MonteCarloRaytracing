#include "SphereMesh.h"

SphereMesh::SphereMesh(glm::vec3 position, float radius)
: position_(position), radius_(radius) {

}

SphereMesh::~SphereMesh() {

}

std::tuple<Mesh::Intersection, float, float> SphereMesh::getIntersections(Ray* ray) {
    glm::vec3 c = position_;
    glm::vec3 o = ray->getOrigin();
    glm::vec3 d = ray->getDirection();

    float denominator = glm::dot(d, d);

    glm::vec3 oMinusC = o - c;
    float numeratorFirstPart = -1 * glm::dot(d, oMinusC);
    float numeratorSecondPart = std::pow(glm::dot(oMinusC, d), 2) - glm::dot(d, d) * (glm::dot(oMinusC, oMinusC) - std::pow(radius_, 2));
    float sMin = 0;
    float sMax = 0;

    if (numeratorSecondPart == 0.0) {
      sMin = numeratorFirstPart / denominator;

      return std::make_tuple(Mesh::Intersection::SINGLE_HIT, sMin, sMax);
    } else if(numeratorSecondPart > 0) {
      sMin = (numeratorFirstPart - std::sqrt(numeratorSecondPart)) / denominator;
      sMax = (numeratorFirstPart + std::sqrt(numeratorSecondPart)) / denominator;

      if (sMax < 0) {
        return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMax, sMin);
      } else {
        return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMin, sMax);
      }
    }

    return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);
}
