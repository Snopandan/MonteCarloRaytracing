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
#include "objects/OpaqueObject.h"

#include "utils/lightning.h"

#include "utils/math.h"
#include "utils/random.h"


class Scene {

public:
  Scene() = default;

  ~Scene();
  
  void add(Object* object);

  std::pair<Object*, glm::vec3> intersect(const Ray* ray) const;
  std::tuple<Object*, glm::vec3, glm::vec3> hit(const Ray* ray) const;

  std::pair<Object*, glm::vec3> intersectNonTransparent(const Ray* ray) const;
  glm::vec3 castShadowRays(const glm::vec3& origin, 
                           const glm::vec2 incomingAngles,
                           Object* object,
                           const unsigned int numberOfShadowRaysToLaunch, const glm::vec3& trueNormal,
                                const glm::vec2& trueNormalAngles) const;

  void complete();

protected:

private:
  std::vector<Object*> objects_;
  std::vector<Object*> lightObjects_;
  std::vector<Object*> transparentObjects_;
  std::vector<Object*> opaqueObjects_;

  inline std::pair<Object*, glm::vec3> intersectImpl(const Ray* ray, const std::vector<Object*>& theObjectVector) const;
  inline std::tuple<Object*, glm::vec3, glm::vec3> hitImpl(const Ray* ray, const std::vector<Object*>& theObjectVector) const;
};

#endif // SCENE_H