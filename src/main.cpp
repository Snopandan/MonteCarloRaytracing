#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <thread>

#include "glm/glm.hpp"

#include "utils/lodepng.h"

#include "Camera.h"
#include "Scene.h"
#include "Ray.h"
#include "objects/meshes/SphereMesh.h"
#include "objects/meshes/BoxMesh.h"
#include "objects/OpaqueObject.h"

#include "ThreadPool.h"
#include "WorkItem.h"


void outputImage(const std::string& file,
                 const  std::vector<unsigned char>& image, 
                 const unsigned int width, 
                 const unsigned int height) {

  unsigned error = lodepng::encode(file.c_str(), image, width, height);

  if( error ) {
    std::ostringstream message;
    message << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
    throw std::domain_error{message.str()};
  }

}



int main(const int argc, const char* argv[]) {

  auto startTime = std::chrono::high_resolution_clock::now();

  const std::string file = "test.png";

  const unsigned int width = 800;
  const unsigned int height = 600;

  Camera camera{glm::ivec2{width, height}, // pixels
                glm::vec2{0.01f, 0.01f},   // pixelSize
                glm::vec3{0, 0, 0},        // position
                1};                        // viewPlaneDistance

  std::vector<Ray*> rays = camera.getRays();





  Scene scene;

  OpaqueObject* boundingBox = new OpaqueObject{new BoxMesh{glm::vec2{-4, 4}, glm::vec2{-3, 3}, glm::vec2{-1.5, 10}}};
  scene.add(boundingBox);

  float box1XTrans = 2.5;
  float box1YTrans = 0.0;
  float box1Width = 0.9;
  OpaqueObject* box1 = new OpaqueObject{new BoxMesh{glm::vec2{-box1Width, box1Width} + glm::vec2{box1XTrans, box1XTrans}, 
                                                    glm::vec2{-box1Width, box1Width} + glm::vec2{box1YTrans, box1YTrans}, 
                                                    glm::vec2{1.1, 1.2}}};
  scene.add(box1);

  OpaqueObject* sphere1 = new OpaqueObject{new SphereMesh{glm::vec3{-8, 0, 5}, 3}};
  scene.add(sphere1);





  ThreadPool threadPool;

  std::vector<unsigned char> image;
  image.resize(width * height * 4);
  std::fill(image.begin(), image.end(), 0);

  for(unsigned x = 0; x < width; x++) {

    WorkItem* workItem = new WorkItem([&, x](){

      for(unsigned y = 0; y < height; y++) {

        std::pair<Object*, glm::vec3> intersection = scene.intersect(rays[width * y + x]);

        int green = 0;
        int red = 0;
        int blue = 0;

        if( intersection.first == boundingBox ) {
          green = 255;
        } if( intersection.first == box1 ) {
          red = 255;
        } if( intersection.first == sphere1 ) {
          blue = 255;
        }

        image[4 * width * y + 4 * x + 0] = red;
        image[4 * width * y + 4 * x + 1] = green;
        image[4 * width * y + 4 * x + 2] = blue;
        image[4 * width * y + 4 * x + 3] = 255;
      }

    });

    threadPool.add(workItem);

  }

  threadPool.wait();

  outputImage(file, image, width, height);




  for(unsigned int i=0; i<rays.size(); i++) {
    delete rays[i];
  }




  auto endTime = std::chrono::high_resolution_clock::now();
  unsigned int duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  std::cout << "DONE: " << duration << " ms" << std::endl;

  return 0;
}
