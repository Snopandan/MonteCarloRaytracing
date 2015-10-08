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
  std::vector<Ray*> rays;
  rays.resize(pixelSize_.x * pixelSize_.y);

  for(int y=pixels_.y/2; y>-pixels_.y/2; y--) {
    for(int x=pixels_.x/2; x>-pixels_.x/2; x--) {
      glm::vec3 pixelUpperLeftCorner{position_.x + x * pixelSize_.x,
                                     position_.y + y * pixelSize_.y,
                                     position_.z + viewPlaneDistance_};

      glm::vec3 rayPosition = pixelUpperLeftCorner - glm::vec3{pixelSize_.x/2.0f, pixelSize_.y/2.0f, 0};
      glm::vec3 rayDirection = glm::normalize(rayPosition - position_);

      // std::cout << rayPosition.x << " " << rayPosition.y << " " << rayPosition.z << std::endl;
      // std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << std::endl;

      rays.push_back(new Ray{rayPosition, rayDirection});
    }
  }

  return rays;
}

