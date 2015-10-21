#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <limits>
#include <stdexcept>

#include "glm/glm.hpp"

#include "objects/Object.h"

class Scene {

public:
  Scene() = default;

  ~Scene();
  
  void add(Object* object);

  std::pair<Object*, glm::vec3> intersect(const Ray* ray) const;
  std::pair<Object*, glm::vec3> intersectNonTransparent(const Ray* ray) const;
  glm::vec3 castShadowRays(const glm::vec3& origin, const unsigned int numberOfShadowRaysToLaunch) const;

  void complete();

protected:

private:
  std::vector<Object*> objects_;
  std::vector<Object*> lightObjects_;
  std::vector<Object*> transparentObjects_;
  std::vector<Object*> opaqueObjects_;

  inline std::pair<Object*, glm::vec3> intersectImpl(const Ray* ray, const std::vector<Object*>& theObjectVector) const;

};

#endif // SCENE_H