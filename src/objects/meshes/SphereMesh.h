#ifndef SPHERE_MESH_H
#define SPHERE_MESH_H

#include <tuple>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Ray.h"

class SphereMesh : public Mesh{
public:
  SphereMesh(glm::vec3 position, float radius);
  virtual ~SphereMesh();

  virtual std::tuple<Mesh::Intersection, float, float> getIntersections(Ray ray);

private:
  glm::vec3 position_;
  float radius_;
};

#endif
