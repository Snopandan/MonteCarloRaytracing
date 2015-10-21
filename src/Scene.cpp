#include "Scene.h"


Scene::~Scene() {
  for(unsigned int i=0; i<objects_.size(); i++) {
    delete objects_[i];
  }
}


void Scene::add(Object* object) {
  objects_.push_back(object);
}


std::pair<Object*, glm::vec3> Scene::intersect(const Ray* ray) const {
  return intersectImpl(ray, objects_);
}


std::pair<Object*, glm::vec3> Scene::intersectNonTransparent(const Ray* ray) const {
  return intersectImpl(ray, opaqueObjects_);
}


void Scene::complete() {
  for(auto& object: objects_) {
    if( object->isLight() ) {
      lightObjects_.push_back(object);
    } 
    if( object->isTransparent() ) {
      transparentObjects_.push_back(object);
    } else {
      opaqueObjects_.push_back(object);
    }
  }

  if( lightObjects_.empty() ) {
    throw std::invalid_argument{"The scene is missing a light source."};
  }
}


std::pair<Object*, glm::vec3> Scene::intersectImpl(const Ray* ray, const std::vector<Object*>& theObjectVector) const {
  const glm::vec3 origin = ray->getOrigin();

  float nearestHitDistance{std::numeric_limits<float>::max()};

  std::pair<Object*, glm::vec3> nearestHit{nullptr, glm::vec3{0}};

  for(auto& object: theObjectVector) {
    std::pair<Object::Intersection, glm::vec3> intersection = object->intersect(ray);
    
    if( intersection.first == Object::Intersection::HIT ) {
      float distance = glm::distance(intersection.second, origin);
      if( distance < nearestHitDistance ) {
        nearestHit = std::pair<Object*, glm::vec3>{object, intersection.second};
      }
    }

  }

  return nearestHit;
}


glm::vec3 Scene::castShadowRays(const glm::vec3& origin, const unsigned int numberOfShadowRaysToLaunch) const {
  static std::default_random_engine generator;
  std::uniform_int_distribution<int> chooseLightDistribution(0, lightObjects_.size()-1);

  glm::vec3 intensity{0, 0, 0};

  for(unsigned int i=0; i<numberOfShadowRaysToLaunch; i++) {
    const unsigned int light = chooseLightDistribution(generator);
    const glm::vec3 randomLightPosition = lightObjects_[light]->getRandomSurfacePosition();

    const Ray* ray = new Ray{origin, glm::normalize(randomLightPosition-origin)};
    const std::pair<Object*, glm::vec3> lightIntersection = intersectImpl(ray, opaqueObjects_);
    if( lightIntersection.first == lightObjects_[light] ) {
      intensity += lightObjects_[light]->getIntensity();
      // intensity += glm::vec3(0, 1, 0);
    }
    delete ray;
  }

  return intensity;
}

