#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <stdexcept>

#include "utils/lodepng.h"

#include "exception/Error.h"


inline void outputImage(const std::string& file,
                 const  std::vector<unsigned char>& image,
                 const unsigned int width,
                 const unsigned int height) {
  unsigned error = lodepng::encode(file.c_str(), image, width, height);
  if( error ) {
    throw std::domain_error{ report_error("Lodepng encoder error " << error << ": "<< lodepng_error_text(error)) };
  }
}


inline glm::vec3 checker(const glm::vec3& uv, const glm::vec3& color0, const glm::vec3& color1) {
  if( (int(floor(uv.x) + floor(uv.y) + floor(uv.z)) & 1 ) == 0) {
    return color0;
  } else {
    return color1;
  }
}


#endif // IMAGE_H