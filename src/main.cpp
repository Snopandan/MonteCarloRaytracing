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

  Camera camera{glm::ivec2{800, 600},    // pixels
                glm::vec2{0.01f, 0.01f}, // pixelSize
                glm::vec3{0, 0, 0},      // position
                1};                      // viewPlaneDistance


  std::vector<Ray*> rays = camera.getRays();

  Scene scene;
  // scene.add(new Plane(glm::vec3(4, 3, 1.1f), glm::vec3(4, -3, 1.1f), glm::vec3(-4, -3, 1.1f)));

  scene.add(new Plane(glm::vec3(4, 3, 10), glm::vec3(4, -3, 10), glm::vec3(-4, -3, 10)));
  // scene.add(new Plane(glm::vec3(4, 3, 10), glm::vec3(4, -3, 10), glm::vec3(-4, -3, 10)));


  unsigned int width = camera.getPixels().x;
  unsigned int height = camera.getPixels().y;
  std::vector<unsigned char> image;
  image.resize(width * height * 4);

  for(unsigned int i=0;i<image.size(); i++) {
    image[i] = 0;
  }
  for(unsigned x = 0; x < width; x++) {
    for(unsigned y = 0; y < height; y++) {
      glm::vec3 intersection;
      int val = scene.intersect(rays[width * y + x], intersection) ? 255 : 0;
      image[4 * width * y + 4 * x + 0] = 0;
      image[4 * width * y + 4 * x + 1] = val;
      image[4 * width * y + 4 * x + 2] = 0;
      image[4 * width * y + 4 * x + 3] = 255;
    }
  }
  

  unsigned error = lodepng::encode("test.png", image, width, height);
  if( error ) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;


  std::cout << "DONE" << std::endl;
  return 0;
}