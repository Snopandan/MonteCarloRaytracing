#include "Camera.h"

Camera::Camera(const glm::ivec2 pixels,
               const glm::vec2 pixelSize,
               const glm::vec3 position, 
               const float viewPlaneDistance) 
: pixels_{pixels}
, pixelSize_{pixelSize}
, position_{position}
, viewPlaneDistance_{viewPlaneDistance}
{

}


glm::vec3 Camera::getPosition() const {
  return position_;
}


float Camera::getViewPlaneDistance() const {
  return viewPlaneDistance_;
}


glm::ivec2 Camera::getPixels() const {  
  return pixels_;
}


glm::vec2 Camera::getPixelSize() const {  
  return pixelSize_;
}


std::vector<Ray*> Camera::getRays() const {
  std::vector<Ray*> rays(pixels_.x * pixels_.y);

  for(int x=pixels_.x/2; x>-pixels_.x/2; x--) {
    for(int y=pixels_.y/2; y>-pixels_.y/2; y--) {
      glm::vec3 pixelUpperLeftCorner{position_.x + x * pixelSize_.x,
                                     position_.y + y * pixelSize_.y,
                                     position_.z + viewPlaneDistance_};

      glm::vec3 rayPosition = pixelUpperLeftCorner - glm::vec3{pixelSize_.x/2, pixelSize_.y/2, 0};
      glm::vec3 rayDirection = glm::normalize(rayPosition - position_);
      
      rays.push_back(new Ray{rayPosition, rayDirection});
    }
    std::cout << x << std::endl;
  }

  return rays;
}

