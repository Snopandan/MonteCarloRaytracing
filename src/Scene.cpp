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

std::tuple<Object*, glm::vec3, glm::vec3> Scene::hit(const Ray* ray) const {
  return hitImpl(ray, objects_);
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
        // nearestHitDistance = distance;
      }
    }

  }

  return nearestHit;
}


std::tuple<Object*, glm::vec3, glm::vec3> Scene::hitImpl(const Ray* ray, const std::vector<Object*>& theObjectVector) const {
  const glm::vec3 origin = ray->getOrigin();

  float nearestHitDistance{std::numeric_limits<float>::max()};

  std::tuple<Object*, glm::vec3, glm::vec3> nearestHit{nullptr, glm::vec3{0}, glm::vec3{0}};

  for(auto& object: theObjectVector) {
    std::tuple<Object::Intersection, glm::vec3, glm::vec3> hit = object->hit(ray);
    
    if( std::get<0>(hit) == Object::Intersection::HIT ) {
      float distance = glm::distance(std::get<1>(hit), origin);
      if( distance < nearestHitDistance ) {
        nearestHit = std::make_tuple(object, std::get<1>(hit), std::get<2>(hit));
        nearestHitDistance = distance;
      }
    }

  }

  return nearestHit;
}


glm::vec3 Scene::castShadowRays(const glm::vec3& origin, 
                                const glm::vec2 incomingAngles,
                                Object* object,
                                const unsigned int numberOfShadowRaysToLaunch,
                                const glm::vec3& trueNormal,
                                const glm::vec2& trueNormalAngles) const {

  glm::vec3 intensity{0, 0, 0};

  for(unsigned int i=0; i<lightObjects_.size(); i++) {

    glm::vec3 contribution{0, 0, 0};
    const float area = lightObjects_[i]->getArea();
    const glm::vec3 light = lightObjects_[i]->getIntensity();

    for(unsigned int r=0; r<numberOfShadowRaysToLaunch; r++) {
      const glm::vec3 randomLightPosition = lightObjects_[i]->getRandomSurfacePosition();
      const glm::vec3 shadowVector = randomLightPosition - origin;
      const Ray* ray = new Ray{origin, glm::normalize(shadowVector)};
      const std::pair<Object*, glm::vec3> intersection = intersectImpl(ray, opaqueObjects_);
      // const std::tuple<Object*, glm::vec3, glm::vec3> hit = hitImpl(ray, opaqueObjects_);

      if( intersection.first == lightObjects_[i] ) {
      // if( std::get<0>(hit) == lightObjects_[i] ) {
        // const glm::vec3 origin = ray->getOrigin();
        const glm::vec3 direction = ray->getDirection();
        const glm::vec3 normal = intersection.first->getNormal(intersection.second);
        // const glm::vec3 normal = std::get<2>(hit);
        float inclination = std::acos(glm::dot(-direction, normal));

         glm::vec2 d1 = {std::acos(direction.z), 
                         std::atan2(direction.y, direction.x)};

        // float inclination = std::acos(glm::dot(-direction, trueNormal));
        // glm::vec3 hemisphereOutgoingDirection = glm::inverse(rotation) * -direction;

         glm::vec2 outgoingAngles = d1 - trueNormalAngles;
        
        const float brdf = dynamic_cast<OpaqueObject*>(object)->computeBrdf(intersection.second, incomingAngles, outgoingAngles);
        // const float brdf = dynamic_cast<OpaqueObject*>(object)->computeBrdf(std::get<1>(hit), incomingAngles, outgoingAngles);

        const float geometric = (std::cos(inclination)*std::cos(outgoingAngles.x) ) / (glm::dot(shadowVector, shadowVector) );

        contribution += brdf * geometric;
        // contribution += brdf;
        // contribution += geometric;
      }

      intensity += (area / (float) numberOfShadowRaysToLaunch) * contribution * light;
      delete ray;
    }

  }

  return intensity;

}

