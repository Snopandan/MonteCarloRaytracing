#include "Camera.h"

Camera::Camera(const glm::ivec2 pixels,
               const glm::vec2 pixelSize,
               const glm::vec3 position, 
               const glm::mat3 rotation,
               const float viewPlaneDistance,
               const unsigned int superSampling) 
: pixels_{pixels}
, pixelSize_{pixelSize}
, position_{position}
, rotation_{rotation}
, viewPlaneDistance_{viewPlaneDistance}
, superSampling_{superSampling}
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


glm::mat3 Camera::getRotation() const {
  return rotation_;
}


unsigned int Camera::getSuperSampling() const {
  return superSampling_;
}


std::vector<Ray*> Camera::getRays() const {
  std::vector<Ray*> rays;
  rays.resize(pixelSize_.x * pixelSize_.y * superSampling_);

  for(int y=pixels_.y/2; y>-pixels_.y/2; y--) {
    for(int x=pixels_.x/2; x>-pixels_.x/2; x--) {

      for(unsigned int s=0; s<superSampling_; s++) {

        const glm::vec3 pixelUpperLeftCorner{position_.x + x * pixelSize_.x,
                                             position_.y + y * pixelSize_.y,
                                             position_.z + viewPlaneDistance_};

        const glm::vec3 rayPosition = pixelUpperLeftCorner - glm::vec3{pixelSize_.x * random0To1(), pixelSize_.y  * random0To1(), 0};
        const glm::vec3 rayDirection = rotation_ * glm::normalize(rayPosition - position_);

        // std::cout << rayPosition.x << " " << rayPosition.y << " " << rayPosition.z << std::endl;
        // std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << std::endl;
        // std::string line;
        // std::getline(std::cin, line);
        rays.push_back(new Ray{rayPosition, rayDirection});
      }

    }
  }

  return rays;
}

