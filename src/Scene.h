#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>

#include "glm/glm.hpp"

#include "objects/SceneObject.h"

class Scene {

public:
  void add(SceneObject* sceneObject);

  bool intersect(Ray* ray, glm::vec3& intersection);

protected:

private:
  std::vector<SceneObject*> sceneObjects_;

};

#endif // SCENE_H