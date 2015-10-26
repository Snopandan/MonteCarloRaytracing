#include "BoundingBoxMesh.h"

BoundingBoxMesh::BoundingBoxMesh(const glm::vec2 xLimits, const glm::vec2 yLimits, const glm::vec2 zLimits) 
: BoxMesh{xLimits, yLimits, zLimits}
{
  bitmap_ = new Bitmap{"assets/wall.bmp"};
  bitmapData_ = bitmap_->get_data();
  bitmapWidth_ = bitmap_->get_width();
  bitmapHeight_ = bitmap_->get_height();
  bitmapSize_ = bitmap_->get_size();
  bitmapChannels_ = bitmap_->get_channels();
}

BoundingBoxMesh::~BoundingBoxMesh() {
  delete bitmap_;
}

glm::vec3 BoundingBoxMesh::getNormal(const glm::vec3& position) const {
  return -1.0f * BoxMesh::getNormal(position);
}


glm::vec3 BoundingBoxMesh::getColor(const glm::vec3& position) const {

  if( equalsEpsilon(position.z, zLimits_.y) ) {
    return glm::vec3(0.5f, 0.8f, 0.5f);
  }

  if( equalsEpsilon(position.x, xLimits_.x) ) {
    glm::vec2 len{zLimits_.y - zLimits_.x, 
                  yLimits_.y - yLimits_.x};

    glm::vec2 uv{(zLimits_.y - position.z) / len.x, 
                 (yLimits_.y - position.y) / len.y};

    uv.y = 1.0f - uv.y;
    unsigned int row = uv.y * bitmapHeight_;
    unsigned int col = uv.x * bitmapWidth_;

    // std::cout << "width: " << bitmapHeight_ << std::endl;
    // std::cout << "height: " << bitmapWidth_ << std::endl;
    // std::cout << "size: " << bitmapSize_ << std::endl;
    // std::cout << "pos: " << row << " " << col << std::endl;

    unsigned int index = (row * bitmapWidth_ + col) * bitmapChannels_;
    // std::cout << "index: " << index << std::endl;
    // unsigned char alfa = bitmapData_[index];
    unsigned char red;   
    unsigned char green;
    unsigned char blue;

    // if( bitmapChannels_ == 3 ) {
      red = bitmapData_[index];
      green = bitmapData_[index + 1];
      blue = bitmapData_[index + 2];
    // } else {
    //   red = bitmapData_[index + 3];
    //   green = bitmapData_[index + 2];
    //   blue = bitmapData_[index + 1];
    // }

    glm::vec3 color{(float)red/(float)255, (float)green/(float)255, (float)blue/(float)255};
    // std::cout << "uv: " << uv.x << " " << uv.y << std::endl;
    // return glm::vec3(uv.y, 1.0f - uv.y, 0.0f);
    return color;
    // return glm::vec3(0.1f, 0.12f, 1.0f);
  }

  if( equalsEpsilon(position.x, xLimits_.y) ) {
    return glm::vec3(1.0f, 0.1f, 0.12f);
  }

  if( equalsEpsilon(position.y, yLimits_.x) ) {
    // glm::vec2 len{zLimits_.y - zLimits_.x, xLimits_.y - xLimits_.x};
    // glm::vec2 uv{(position.z / len.x), (position.x / len.y)};
    const glm::vec3 color1 = glm::vec3{0.9f, 0.9, 0.9f};
    const glm::vec3 color2 = glm::vec3{0.1f, 0.1f, 0.1f};
    return checker(position, color1, color2);
  }

  if( equalsEpsilon(position.y, yLimits_.y) ) {
    return glm::vec3(1.0f, 0.968627451f, 0.8f);
  }

  // return glm::vec3(0.1f, 0.12f, 1.0f);
  return glm::vec3(1.0f, 1.0f, 0.1f);

}
