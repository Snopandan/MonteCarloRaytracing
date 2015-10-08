#ifndef OBJECT_H
#define OBJECT_H

#include <utility>

#include <glm/glm.hpp>

#include "Ray.h"
#include "meshes/Mesh.h"

class Object {
public:
  enum class Intersection {MISS, HIT};

  Object(Mesh* mesh, bool isTransparent);
  virtual ~Object() = 0;

  bool isTransparent() const { return isTransparent_; }

  virtual std::pair<Object::Intersection, glm::vec3> intersect(Ray ray) const;

protected:
  Mesh* mesh_;

private:
  const bool isTransparent_;
};
#endif
