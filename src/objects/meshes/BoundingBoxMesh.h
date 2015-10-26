#ifndef BOUNDINGBOXMESH_H
#define BOUNDINGBOXMESH_H

#include <algorithm>

#include "glm/glm.hpp"

#include "BoxMesh.h"
#include "format/Bitmap.h"
#include "utils/math.h"

class BoundingBoxMesh : public BoxMesh {

public:
  BoundingBoxMesh(const glm::vec2 xLimits, const glm::vec2 yLimits, const glm::vec2 zLimits);

  ~BoundingBoxMesh();

  virtual glm::vec3 getNormal(const glm::vec3& position) const override;

  virtual glm::vec3 getColor(const glm::vec3& position) const override;
  
private:
  Bitmap* bitmap_;
  unsigned char* bitmapData_;
  unsigned int bitmapWidth_;
  unsigned int bitmapHeight_;
  unsigned int bitmapSize_;
  unsigned int bitmapChannels_;
};


#endif
