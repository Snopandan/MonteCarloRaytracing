#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

#include "glm/glm.hpp"

#include "Mesh.h"

#define EPSILON 0.000001


class TriangleMesh : public Mesh {

public:
  TriangleMesh(const std::vector<glm::vec3> verticies);

  std::tuple<Mesh::Intersection, float, float> getIntersections(const Ray* ray) const override;

protected:

private:
  const std::vector<glm::vec3> verticies_;

  bool triangleIntersection(const Ray* ray, const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3, glm::vec3& intersection) const;

};


#endif // TRIANGLEMESH_H