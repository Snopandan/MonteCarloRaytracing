#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

#include "glm/glm.hpp"

#include "Mesh.h"

#include "utils/random.h"

#define EPSILON 0.000001


class TriangleMesh : public Mesh {

public:
  TriangleMesh(const std::vector<glm::vec3> verticies, const std::vector<glm::vec3> normals = std::vector<glm::vec3>{});

  virtual ~TriangleMesh() = default;

  std::tuple<Mesh::Intersection, float, float> getIntersections(const Ray* ray) const override;

  glm::vec3 getNormal(const glm::vec3& position) const override;

protected:

private:
  inline float sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) const {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
  }

  const std::vector<glm::vec3> verticies_;
  const std::vector<glm::vec3> normals_;
  const bool hasNormals_;

  bool triangleIntersection(const Ray* ray, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, glm::vec3& intersection, glm::vec3& normal, float& t) const;

};


#endif // TRIANGLEMESH_H