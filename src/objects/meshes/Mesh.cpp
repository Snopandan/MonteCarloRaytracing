#include "Mesh.h"

Mesh::Mesh(){

}

Mesh::~Mesh(){

}


std::tuple<Mesh::Intersection, glm::vec3, glm::vec3> Mesh::hit(const Ray* ray) const {
  
  std::tuple<Mesh::Intersection, float, float> intersections = getIntersections(ray);

  glm::vec3 intersection{0.0f, 0.0f, 0.0f};
  glm::vec3 normal{0.0f, 0.0f, 0.0f};

  if( std::get<0>(intersections) == Mesh::Intersection::MISS ) {
   return std::make_tuple(Mesh::Intersection::MISS, intersection, normal);
  }

  intersection = ray->getOrigin() + std::get<1>(intersections) * ray->getDirection();
  normal = getNormal(intersection);
  
  return std::make_tuple(Mesh::Intersection::SINGLE_HIT, intersection, normal);
}
