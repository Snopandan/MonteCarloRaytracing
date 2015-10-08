#include "Scene.h"


void Scene::add(SceneObject* sceneObject) {
  sceneObjects_.push_back(sceneObject);
}


bool Scene::intersect(Ray* ray, glm::vec3& intersection) {
  for(auto& sceneObject: sceneObjects_) {
    if( sceneObject->intersect(ray, intersection) ) {
      return true;
    }
  }
  return false;
}
