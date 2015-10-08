#ifndef MESH_H
#define MESH_H

#include <tuple>

#include <glm/glm.hpp>

#include "Ray.h"

class Mesh {
public:
  enum class Intersection {MISS, SINGLE_HIT, DOUBLE_HIT};

  Mesh();
  virtual ~Mesh() = 0;

  virtual std::tuple<Mesh::Intersection, float, float> getIntersections(Ray* ray) = 0;

private:

};

#endif
