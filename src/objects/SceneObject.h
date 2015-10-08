#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "glm/glm.hpp"

#include "Ray.h"

class SceneObject {

public:

  virtual bool intersect(Ray* ray, glm::vec3& intersection) = 0;

protected:

private:

};

#endif // SCENEOBJECT_H