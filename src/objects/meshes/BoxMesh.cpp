#include "BoxMesh.h"


BoxMesh::BoxMesh(const glm::vec2 xLimits, const glm::vec2 yLimits, const glm::vec2 zLimits) 
: xLimits_{xLimits}
, yLimits_{yLimits}
, zLimits_{zLimits}
{

}


std::tuple<Mesh::Intersection, float, float> BoxMesh::getIntersections(Ray ray) {

  // dmnsn_optimized_line optline, dmnsn_bounding_box box, double t

  glm::vec3 origin = ray.getOrigin();
  glm::vec3 direction = ray.getDirection();
  glm::vec3 inversedDirection = ray.getInversedDirection();


  double tx1 = (xLimits_.x - origin.x) * inversedDirection.x;
  double tx2 = (xLimits_.y - origin.x) * inversedDirection.x;

  double sMin = tx1 < tx2 ? tx1 : tx2;
  double sMax = tx1 > tx2 ? tx1 : tx2;

  double ty1 = (yLimits_.x - origin.y) * inversedDirection.y;
  double ty2 = (yLimits_.y - origin.y) * inversedDirection.y;

  sMin = sMin > (ty1 < ty2 ? ty1 : ty2) ? sMin : (ty1 < ty2 ? ty1 : ty2);
  sMax = sMax < (ty1 > ty2 ? ty1 : ty2) ? sMax : (ty1 > ty2 ? ty1 : ty2);

  double tz1 = (zLimits_.x - origin.z) * inversedDirection.z;
  double tz2 = (zLimits_.y - origin.z) * inversedDirection.z;

  sMin = sMin > (tz1 < tz2 ? tz1 : tz2) ? sMin : (tz1 < tz2 ? tz1 : tz2);
  sMax = sMax < (tz1 > tz2 ? tz1 : tz2) ? sMax : (tz1 > tz2 ? tz1 : tz2);

  bool hit = sMax >= (0.0 > sMin ? 0.0 : sMin);// && sMin < t;

  // if( hit ) {
  //   return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMin, sMax);
  // }

  if( hit && sMin < 0 && sMax > 0) {
    // Innuti 
    return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMax, sMin);
  }

  if( hit && sMin > 0 && sMax > 0 ) {
    // Boxen framför oss
    return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMin, sMax);
  }

  // if( sMin < 0 && sMax < 0 ) {
    // Boxen bakom oss
    return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);
  // }



  // return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);

}
