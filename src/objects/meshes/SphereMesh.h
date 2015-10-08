#ifndef SPHERE_MESH
#define SPHERE_MESH

#include <tuple>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Ray.h"

class SphereMesh : public Mesh{
public:
  SphereMesh(const glm::vec3 position, const float radius);
  virtual ~SphereMesh();

  std::tuple<Mesh::Intersection, float, float> getIntersections(const Ray* ray) const override;

private:
  const glm::vec3 position_;
  const float radius_;
};

#endif
