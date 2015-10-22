#include "OrtPlaneMesh.h"


OrtPlaneMesh::OrtPlaneMesh(const glm::vec3 upperLeftCorner, 
                           const glm::vec3 lowerLeftCorner,
                           const glm::vec3 lowerRightCorner)
: upperLeftCorner_{upperLeftCorner}
, lowerLeftCorner_{lowerLeftCorner}
, lowerRightCorner_{lowerRightCorner}
, upperRightCorner_{lowerRightCorner + upperLeftCorner - lowerLeftCorner}
{ 

  edge1_ = lowerLeftCorner_ - upperLeftCorner_;
  edge2_ = lowerRightCorner_ - upperLeftCorner_; 

  edge3_ = upperLeftCorner_ - lowerLeftCorner_;
  edge4_ = lowerRightCorner_ - lowerLeftCorner_; 

  normal_ = glm::normalize(glm::cross(edge1_, edge2_));
  center_ = lowerLeftCorner_ + edge3_/2.0f + edge4_/2.0f;

  xLimits_ = glm::vec2{std::min(std::min(upperLeftCorner_.x, lowerLeftCorner_.x), lowerRightCorner_.x) - getEpsilon(), 
                       std::max(std::max(upperLeftCorner_.x, lowerLeftCorner_.x), lowerRightCorner_.x) + getEpsilon()};
  yLimits_ = glm::vec2{std::min(std::min(upperLeftCorner_.y, lowerLeftCorner_.y), lowerRightCorner_.y) - getEpsilon(), 
                       std::max(std::max(upperLeftCorner_.y, lowerLeftCorner_.y), lowerRightCorner_.y) + getEpsilon()}; 
  zLimits_ = glm::vec2{std::min(std::min(upperLeftCorner_.z, lowerLeftCorner_.z), lowerRightCorner_.z) - getEpsilon(), 
                       std::max(std::max(upperLeftCorner_.z, lowerLeftCorner_.z), lowerRightCorner_.z) + getEpsilon()};

  // std::cout << "xLimits_: " << xLimits_.x << " " << xLimits_.y << std::endl;
  // std::cout << "yLimits_: " << yLimits_.x << " " << yLimits_.y << std::endl;
  // std::cout << "zLimits_: " << zLimits_.x << " " << zLimits_.y << std::endl;
  // std::cout << "center_: " << center_.x << " " << center_.y << " " << center_.z << std::endl;
  // std::cout << "normal_: " << normal_.x << " " << normal_.y << " " << normal_.z << std::endl;
}


std::tuple<Mesh::Intersection, float, float> OrtPlaneMesh::getIntersections(const Ray* ray) const {

  const glm::vec3 origin = ray->getOrigin();
  const glm::vec3 direction = ray->getDirection();

  float sMin = 0.0f;
  float sMax = 1.0f;
    
  // if( equalsEpsilon(glm::dot(normal_, direction), 0.0f) ) { // Perpendicular ray
  //   return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);
  // }
   
  if( glm::dot(normal_, -direction) <= getEpsilon() ) { // Backface culling
    return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);
  }

  // const glm::vec3 p0 = origin;
  // const glm::vec3 p1 = origin + direction;
  // const glm::vec3 p2 = center_;
  // const glm::vec3 n = normal_;

  // float t = glm::dot(n, p2-p0) / glm::dot(n, p1-p0);
  // const glm::vec3 px = p0 + t * (p1-p0);

  const float t = (glm::dot(normal_, center_-origin) / glm::dot(normal_, direction));

  if( t <= 0 ) {
    return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);
  }

  const glm::vec3 point = origin + t * (direction);

  // std::cout << "point: " << point.x << " " << point.y << " " << point.z << std::endl;

  if( (xLimits_.x <= point.x) && (point.x <= xLimits_.y) &&
      (yLimits_.x <= point.y) && (point.y <= yLimits_.y) &&
      (zLimits_.x <= point.z) && (point.z <= zLimits_.y) && 
      equalsEpsilon(glm::dot(normal_, center_-point), 0.0f) ) {

    // std::cout << "HIT!" << std::endl;
    // std::cout << "origin: " << origin.x << " " << origin.y << " " << origin.z << std::endl;
    // std::cout << "direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
    // std::cout << "point: " << point.x << " " << point.y << " " << point.z << std::endl;

    return std::make_tuple(Mesh::Intersection::SINGLE_HIT, sMin, sMax);
  }

  // std::cout << "MISS!" << std::endl;
  return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);


  // http://www.xbdev.net/maths_of_3d/collision_detection/line_with_plane/index.php
  // N dot (Px - P2) = 0
  // N dot { P0+t(P1-P0) - P2 } = 0
  // N dot { P0+t(P1-P0) } - N dot (P2) = 0
  // N dot { P0+t(P1-P0) } = N dot (P2)
  // N dot (P0) + N dot { t(P1-P0) } = N dot (P2)
  // N dot { t(P1-P0) } = N dot { P2-P0 }
  // t is a constant so
  //    t * N dot { P1-P0 } = N dot { P2-P0 }
  // t =  N dot {P2-P0} /  N dot {P1-P0}

  // t > 0 and t < 1     :  The intersection occurs between the two end points
  // t = 0               :   The intersection falls on the first end point
  // t = 1               :   Intersection falls on the second end point
  // t > 1               :   Intersection occurs beyond second end Point
  // t < 0               :   Intersection happens before 1st end point.



  // return std::make_tuple(Mesh::Intersection::DOUBLE_HIT, sMin, sMax);
  // return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);

}


glm::vec3 OrtPlaneMesh::getNormal(const glm::vec3& position) const {
  return normal_;
}

glm::vec3 OrtPlaneMesh::getRandomSurfacePosition() const {
  return lowerLeftCorner_ + random0To1() * edge3_ + random0To1() * edge4_;
}

