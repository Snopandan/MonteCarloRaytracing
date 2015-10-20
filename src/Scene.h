#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>

#include "glm/glm.hpp"

#include "objects/Object.h"

class Scene {

public:
  Scene() = default;

  ~Scene();
  
  void add(Object* object);

  std::pair<Object*, glm::vec3> intersect(const Ray* ray) const;

protected:

private:
  std::vector<Object*> objects_;

};

#endif // SCENE_H