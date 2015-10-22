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
      // std::cout << "randomLightPosition: " << randomLightPosition.x << " " << randomLightPosition.y << " " << randomLightPosition.z << std::endl;
    // assert( randomLightPosition.x >= -2 && randomLightPosition.x <= 2 );
    // assert( randomLightPosition.y >= -2 && randomLightPosition.y <= 2 );
    // assert( randomLightPosition.z >= -4.5 );

    const Ray* ray = new Ray{origin, glm::normalize(randomLightPosition-origin)};
    const std::pair<Object*, glm::vec3> lightIntersection = intersectImpl(ray, opaqueObjects_);

    if( lightIntersection.first == lightObjects_[light] ) {

      const glm::vec3 origin = ray->getOrigin();
      const glm::vec3 direction = ray->getDirection();
      const glm::vec3 normal = lightIntersection.first->getNormal(lightIntersection.second);
      const glm::vec3 viewDirection = glm::vec3(origin);

      // TODO: Material properties
      const float ka = 0.3f;
      const float kd = 0.7f;
      const float ks = 0.2f;
      const glm::vec3 diffuseColor = glm::vec3(0.9f, 0.9f, 0.9f);;
      const glm::vec3 ambientColor = diffuseColor;
      const glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
      const float specularity = 5;

      glm::vec3 color = localLightning(origin, 
                                       direction, 
                                       normal, 
                                       viewDirection,
                                       ka,
                                       kd,
                                       ks,
                                       ambientColor,
                                       diffuseColor,
                                       specularColor,
                                       specularity);

      intensity += color * lightObjects_[light]->getIntensity();
      // intensity += glm::vec3(0, 1, 0);
    } 
    delete ray;
  }

  return intensity;
}

