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


void Scene::complete() {
  for(auto& object: objects_) {
    if( object->isLight() ) {
      lightObjects_.push_back(object);
    } 
    if( object->isTransparent() && dynamic_cast<TransparentObject*>(object)->getTransparancy() > 0.0f ) {
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
      // const float distance = glm::distance(intersection.second, origin);
      const float distance = glm::length(intersection.second - origin);
      if( distance < nearestHitDistance ) {
        nearestHit = std::pair<Object*, glm::vec3>{object, intersection.second};
        // nearestHit = std::make_pair(object, intersection.second);
        nearestHitDistance = distance;
         // return nearestHit;
      }
    }

  }

  return nearestHit;
}


glm::vec3 Scene::castShadowRays(const glm::vec3& trueOrigin, 
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
      const glm::vec3 shadowVector = randomLightPosition - trueOrigin;
      const Ray* ray = new Ray{trueOrigin, glm::normalize(shadowVector)};
      const std::pair<Object*, glm::vec3> intersection = intersectImpl(ray, opaqueObjects_);
      // const std::tuple<Object*, glm::vec3, glm::vec3> hit = hitImpl(ray, opaqueObjects_);

      if( intersection.first == lightObjects_[i] ) {
      // if( std::get<0>(hit) == lightObjects_[i] ) {
        // const glm::vec3 origin = ray->getOrigin();
        const glm::vec3 direction = ray->getDirection();
        const glm::vec3 normal = intersection.first->getNormal(intersection.second);
        // const glm::vec3 normal = std::get<2>(hit);
        float inclination = std::acos(glm::dot(-direction, normal));
        // glm::vec3 directionFlipped = -direction;
        // glm::vec2 d0 = {std::acos(directionFlipped.z), 
        //                  std::atan2(directionFlipped.y, directionFlipped.x)};
        // glm::vec2 d5 = {std::acos(normal.z), 
        //          std::atan2(normal.y, normal.x)};
        // float inclination = d5.x - d0.x;

         glm::vec2 d1 = {std::acos(direction.z), 
                         std::atan2(direction.y, direction.x)};
         // glm::vec2 fakeNormalAngels = {std::acos(normal.z), 
         //                 std::atan2(normal.y, normal.x)};

        // float inclination = std::acos(glm::dot(-direction, trueNormal));
        // glm::vec3 hemisphereOutgoingDirection = glm::inverse(rotation) * -direction;

         glm::vec2 outgoingAngles = d1 - trueNormalAngles;
         // glm::vec2 outgoingAngles = d1 - fakeNormalAngels;
        
        // const float brdf = dynamic_cast<OpaqueObject*>(object)->computeBrdf(intersection.second, incomingAngles, outgoingAngles);
        const float brdf = dynamic_cast<OpaqueObject*>(object)->computeBrdf(trueOrigin, incomingAngles, outgoingAngles);
        // const float brdf = dynamic_cast<OpaqueObject*>(object)->computeBrdf(std::get<1>(hit), incomingAngles, outgoingAngles);

        const float geometric = (std::cos(inclination)*std::cos(outgoingAngles.x) ) / (glm::dot(shadowVector, shadowVector) );

        contribution += brdf * geometric;
        // contribution += brdf;
        // contribution += geometric;
      } 
      // else {
      //   contribution = glm::vec3{-0.5f, -0.5f, -0.5f};
      // }

      intensity += (area / (float) numberOfShadowRaysToLaunch) * contribution * light;
      delete ray;
    }

  }

  return intensity;

}

