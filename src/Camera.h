#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "glm/glm.hpp"

#include "Ray.h"


class Camera {

public:
  Camera(const glm::ivec2 pixels,
         const glm::vec2 pixelSize,
         const glm::vec3 position, 
         const float viewPlaneDistance);

  glm::vec3 getPosition() const;

  float getViewPlaneDistance() const;

  glm::ivec2 getPixels() const;

  glm::vec2 getPixelSize() const;

  std::vector<Ray*> getRays() const;

protected:

private:
  const glm::ivec2 pixels_;
  const glm::vec2 pixelSize_;
  const glm::vec3 position_;
  const float viewPlaneDistance_;

};


#endif // CAMERA_H