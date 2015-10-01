#include "Scene.h"


void Scene::add(SceneObject* sceneObject) {
  sceneObjects_.push_back(sceneObject);
}


bool Scene::intersect(Ray* ray) {
  for(auto& sceneObject: sceneObjects_) {
    if( sceneObject->intersect(ray) ) {
      return true;
    }
  }
  return false;
}
