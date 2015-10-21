#ifndef ORTPLANEMESH_H
#define ORTPLANEMESH_H

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <tuple>

#include "glm/glm.hpp"

#include "Mesh.h"
#include "utils/random.h"


class OrtPlaneMesh : public Mesh {

public:
  OrtPlaneMesh(const glm::vec3 upperLeftCorner, 
               const glm::vec3 lowerLeftCorner,
               const glm::vec3 lowerRightCorner);

  std::tuple<Mesh::Intersection, float, float> getIntersections(const Ray* ray) const override;
  glm::vec3 getNormal(const glm::vec3& position) const override;
  glm::vec3 getRandomSurfacePosition() const override;

protected:

private:
  glm::vec3 upperLeftCorner_;
  glm::vec3 lowerLeftCorner_;
  glm::vec3 lowerRightCorner_;
  glm::vec3 upperRightCorner_;

  glm::vec3 normal_;
  glm::vec3 center_;

  glm::vec3 edge1_;
  glm::vec3 edge2_;

  glm::vec2 xLimits_;
  glm::vec2 yLimits_;
  glm::vec2 zLimits_;

};



#endif