#ifndef SPHERE_MESH_H
#define SPHERE_MESH_H

#include <tuple>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Ray.h"


class SphereMesh : public Mesh {

public:
  SphereMesh(const glm::vec3 position, const float radius);
  ~SphereMesh();

  std::tuple<Mesh::Intersection, float, float> getIntersections(const Ray* ray) const override;

private:
  const glm::vec3 position_;
  const float radius_;
  const float radiusPow2_;

};


#endif // SPHERE_MESH_H
