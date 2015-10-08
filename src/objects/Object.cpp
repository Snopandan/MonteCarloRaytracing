#include "Object.h"

Object::Object(Mesh* mesh, bool isTransparent)
: mesh_(mesh), isTransparent_(isTransparent) {

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
