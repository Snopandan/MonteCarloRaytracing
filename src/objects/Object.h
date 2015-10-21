#ifndef OBJECT_H
#define OBJECT_H

#include <utility>

#include <glm/glm.hpp>

#include "Ray.h"
#include "meshes/Mesh.h"

class Object {
public:
  enum class Intersection {MISS, HIT};

  Object(Mesh* mesh, const bool isTransparent = false, const bool isLight = false);
  virtual ~Object() = 0;

  bool isTransparent() const { return isTransparent_; }
  bool isLight() const { return isLight_; }

  virtual std::pair<Object::Intersection, glm::vec3> intersect(const Ray* ray) const;
  virtual glm::vec3 getNormal(const glm::vec3& position) const { return mesh_->getNormal(position); }

protected:
  Mesh* mesh_;

private:
  const bool isTransparent_;
  const bool isLight_;
};
#endif
