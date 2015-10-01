#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Ray.h"

class SceneObject {

public:

  virtual bool intersect(Ray* ray) = 0;

protected:

private:

};

#endif // SCENEOBJECT_H