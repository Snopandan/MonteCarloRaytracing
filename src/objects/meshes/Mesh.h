#ifndef MESH_H
#define MESH_H

#include <tuple>
#include <stdexcept>

#include <glm/glm.hpp>

#include "Ray.h"

class Mesh {
public:
  enum class Intersection {MISS, SINGLE_HIT, DOUBLE_HIT};

  Mesh();
  virtual ~Mesh() = 0;

  virtual std::tuple<Mesh::Intersection, float, float> getIntersections(const Ray* ray) const = 0;
  virtual glm::vec3 getNormal(const glm::vec3& position) const = 0;
  virtual glm::vec3 getRandomSurfacePosition() const { throw std::invalid_argument{"getRandomSurfacePosition() not implemented"};
                                                       return glm::vec3{0,0,0}; }
  virtual float getArea() const { throw std::invalid_argument{"getArea() not implemented"}; return 1.0f; }

private:

};

#endif
