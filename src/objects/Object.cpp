#include "Object.h"

Object::Object(Mesh* mesh, const bool isTransparent, const bool isLight)
: mesh_(mesh), isTransparent_(isTransparent), isLight_{isLight} {

}

Object::~Object() {
  delete mesh_;
}

std::pair<Object::Intersection, glm::vec3> Object::intersect(const Ray* ray) const {
  std::tuple<Mesh::Intersection, float, float> intersections = mesh_->getIntersections(ray);

  if (std::get<0>(intersections) == Mesh::Intersection::MISS) {
    return std::make_pair(Object::Intersection::MISS, glm::vec3{0, 0, 0});
  }

  glm::vec3 hitLocation = ray->getOrigin() + std::get<1>(intersections) * ray->getDirection();
  return std::make_pair(Object::Intersection::HIT, hitLocation);
}


std::tuple<Object::Intersection, glm::vec3, glm::vec3> Object::hit(const Ray* ray) const {
  std::tuple<Mesh::Intersection, glm::vec3, glm::vec3> tup = mesh_->hit(ray);

  if( std::get<0>(tup) == Mesh::Intersection::MISS ) {
    return std::make_tuple(Object::Intersection::MISS, glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0});
  }
  return std::make_tuple(Object::Intersection::HIT, std::get<1>(tup), std::get<2>(tup));
}
