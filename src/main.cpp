#include <iostream>
#include <vector>
#include <algorithm>

#include "glm/glm.hpp"

#include "utils/lodepng.h"

#include "Camera.h"
#include "Scene.h"
#include "objects/Plane.h"
#include "Ray.h"

int main(const int argc, const char* argv[]) {

  Camera camera{glm::ivec2{800, 600},
                glm::vec2{1, 1},
                glm::vec3{100, 50, 0}, 
                100};


  std::vector<Ray*> rays = camera.getRays();

  Scene scene;
  scene.add(new Plane());

  int val = 0;
  for(auto& ray : rays) {
    if( scene.intersect(ray) ) {
      val = 255;
    }
  }
  

  unsigned width = 512, height = 512;
  std::vector<unsigned char> image;
  image.resize(width * height * 4);
  for(unsigned y = 0; y < height; y++)
  for(unsigned x = 0; x < width; x++)
  {
    image[4 * width * y + 4 * x + 0] = 0;
    image[4 * width * y + 4 * x + 1] = val;
    image[4 * width * y + 4 * x + 2] = 0;
    image[4 * width * y + 4 * x + 3] = 255;
  }

  unsigned error = lodepng::encode("test.png", image, width, height);
  if( error ) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;


  std::cout << "DONE" << std::endl;
  return 0;
}