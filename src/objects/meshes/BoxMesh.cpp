#include "BoxMesh.h"


BoxMesh::BoxMesh(const glm::vec2 xLimits, const glm::vec2 yLimits, const glm::vec2 zLimits)
: xLimits_{xLimits}
, yLimits_{yLimits}
, zLimits_{zLimits}
{

}


std::tuple<Mesh::Intersection, float, float> BoxMesh::getIntersections(const Ray* ray) const {

  const glm::vec3 origin = ray->getOrigin();
  const glm::vec3 inversedDirection = ray->getInversedDirection();


  const double tx1 = (xLimits_.x - origin.x) * inversedDirection.x;
  const double tx2 = (xLimits_.y - origin.x) * inversedDirection.x;

  double sMin = tx1 < tx2 ? tx1 : tx2;
  double sMax = tx1 > tx2 ? tx1 : tx2;

  const double ty1 = (yLimits_.x - origin.y) * inversedDirection.y;
  const double ty2 = (yLimits_.y - origin.y) * inversedDirection.y;

  sMin = sMin > (ty1 < ty2 ? ty1 : ty2) ? sMin : (ty1 < ty2 ? ty1 : ty2);
  sMax = sMax < (ty1 > ty2 ? ty1 : ty2) ? sMax : (ty1 > ty2 ? ty1 : ty2);

  const double tz1 = (zLimits_.x - origin.z) * inversedDirection.z;
  const double tz2 = (zLimits_.y - origin.z) * inversedDirection.z;

  sMin = sMin > (tz1 < tz2 ? tz1 : tz2) ? sMin : (tz1 < tz2 ? tz1 : tz2);
  sMax = sMax < (tz1 > tz2 ? tz1 : tz2) ? sMax : (tz1 > tz2 ? tz1 : tz2);

  const bool hit = sMax >= (0.0 > sMin ? 0.0 : sMin);

  if( hit && sMin < 0 && sMax > 0) {
    // Innuti
    // std::cout << "Innuti" << std::endl;

    return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMax, sMin);
  }

  if( hit && sMin > 0 && sMax > 0 ) {
    // Boxen framför oss
    // std::cout << "Framfor" << std::endl;

    return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMin, sMax);
  }

    // std::cout << "BAKOm" << std::endl;

  // if( sMin < 0 && sMax < 0 ) {
    // Boxen bakom oss
    return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);
  // }

}

glm::vec3 BoxMesh::getNormal(const glm::vec3& position) const {

  // Bakom
  if( equalsEpsilon(position.x, xLimits_.x) ) {
    return glm::vec3(-1.0f, 0.0f, 0.0f);
  }

  // Framfor
  if ( equalsEpsilon(position.x, xLimits_.y) ) {
    return glm::vec3(1.0f, 0.0f, 0.0f);
  }

  // Hoger
  if( equalsEpsilon(position.y, yLimits_.x) ) {
    return glm::vec3(0.0f, -1.0f, 0.0f);
  }

  // Vänster
  if( equalsEpsilon(position.y, yLimits_.y) ) {
    return glm::vec3(0.0f, 1.0f, 0.0f);
  }

  // Undre
  if( equalsEpsilon(position.z, zLimits_.x) ) {
    return glm::vec3(0.0f, 0.0f, -1.0f);
  }

  // ÖVre
  if( equalsEpsilon(position.z, zLimits_.y) ) {
    return glm::vec3(0.0f, 0.0f, 1.0f);
  }

  throw std::invalid_argument{"The given position is not on the surface of the BoxMesh."};
}
