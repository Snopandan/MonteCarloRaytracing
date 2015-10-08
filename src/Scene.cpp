#include "Scene.h"


Scene::~Scene() {
  for(unsigned int i=0; i<objects_.size(); i++) {
    delete objects_[i];
  }
}


void Scene::add(Object* object) {
  objects_.push_back(object);
}


std::pair<Object*, glm::vec3> Scene::intersect(Ray* ray) {
  glm::vec3 origin = ray->getOrigin();

  float nearestHitDistance{std::numeric_limits<float>::max()};

  std::pair<Object*, glm::vec3> nearestHit{nullptr, glm::vec3{0}};

  for(auto& sceneObject: objects_) {
    std::pair<Object::Intersection, glm::vec3> intersection = sceneObject->intersect(*ray);
    
    if( intersection.first == Object::Intersection::HIT ) {
      float distance = glm::distance(intersection.second, origin);
      if( distance < nearestHitDistance ) {
        nearestHit = std::pair<Object*, glm::vec3>{sceneObject, intersection.second};
      }
    }

  }

  return nearestHit;
}
