#ifndef BOXMESH_H
#define BOXMESH_H

#include <algorithm>

#include "glm/glm.hpp"

#include "Mesh.h"

class BoxMesh : public Mesh {

public:
  BoxMesh(const glm::vec2 xLimits, const glm::vec2 yLimits, const glm::vec2 zLimits);

  std::tuple<Mesh::Intersection, float, float> getIntersections(const Ray* ray) const override;

protected:

private:
  const glm::vec2 xLimits_;
  const glm::vec2 yLimits_;
  const glm::vec2 zLimits_;

};

#endif // BOXMESH_H