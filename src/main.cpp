#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <thread>
#include <functional>

#include "glm/glm.hpp"

#include "utils/lodepng.h"

#include "Camera.h"
#include "Scene.h"
#include "Ray.h"
#include "objects/meshes/SphereMesh.h"
#include "objects/meshes/BoxMesh.h"
#include "objects/meshes/BoundingBoxMesh.h"
#include "objects/meshes/TriangleMesh.h"
#include "objects/meshes/PlaneMesh.h"
#include "objects/OpaqueObject.h"
#include "objects/TransparentObject.h"
#include "objects/brdfs/BrdfLambertian.h"


#include "thread/ThreadPool.h"
#include "thread/WorkItem.h"

#include "utils/Node.h"
#include "utils/random.h"


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

  OpaqueObject* boundingBox = new OpaqueObject{new BoundingBoxMesh{glm::vec2{-10, 10}, glm::vec2{-10, 10}, glm::vec2{-10, 10}},
    new BrdfLambertian{0.5f}};
  scene.add(boundingBox);

  // float box1XTrans = 2.5;
  // float box1YTrans = 0.0;
  // float box1Width = 0.9;
  // OpaqueObject* box1 = new OpaqueObject{new BoxMesh{glm::vec2{-box1Width, box1Width} + glm::vec2{box1XTrans, box1XTrans},
  //                                                   glm::vec2{-box1Width, box1Width} + glm::vec2{box1YTrans, box1YTrans},
  //                                                   glm::vec2{1.1, 1.2}}};
  // scene.add(box1);

  OpaqueObject* sphere1 = new OpaqueObject{new SphereMesh{glm::vec3{-8, 0, 5}, 3},
    new BrdfLambertian{1.0f}};
  // scene.add(sphere1);


  // std::vector<glm::vec3> verticies;
  // verticies.push_back(glm::vec3{-4, 0, 7});
  // verticies.push_back(glm::vec3{8, 0, 7});
  // verticies.push_back(glm::vec3{0, 9, 7});
  // OpaqueObject* triangle1 = new OpaqueObject{new TriangleMesh{verticies}};
  // scene.add(triangle1);
  //
  // OpaqueObject* plane1 = new OpaqueObject{new PlaneMesh{glm::vec3{-8, 15, 5.5}, glm::vec3{-8, 15, 10}, glm::vec3{4, 15, 10} }};
  // scene.add(plane1);






  std::vector<unsigned char> image;
  image.resize(width * height * 4);
  std::fill(image.begin(), image.end(), 0);

  for(unsigned x = 0; x < width; x++) {
      for(unsigned y = 0; y < height; y++) {

        const float rootImportance = 1.0f;
        Node* root = new Node{rays[width * y + x], rootImportance};

        std::function<void(Node*)> traverse = [&scene, &traverse](Node* node) {
          Ray* ray = node->getRay();
          std::pair<Object*, glm::vec3> intersection = scene.intersect(ray);
          if( intersection.first == nullptr) {
            std::cout << "hej" << std::endl;
          }

          if( intersection.first != nullptr && !intersection.first->isLight() ) {
            if( intersection.first->isTransparent() ) {
              const glm::vec3 direction = ray->getDirection();
              const glm::vec3 normal = intersection.first->getNormal(intersection.second);
              const glm::vec3 reflection = glm::reflect(direction, normal);
              const float ratio = 1.0f; // TODO: Get from object
              const glm::vec3 refraction = glm::refract(direction, normal, ratio);
              const float importance = node->getImportance();
              const float transparency = dynamic_cast<TransparentObject*>(intersection.first)->getTransparancy();

              node->setReflected(new Node{new Ray{intersection.second, reflection}, importance * (1 - transparency)});
              node->setRefracted(new Node{new Ray{intersection.second, refraction}, importance * transparency});

              traverse(node->getReflected());
              traverse(node->getRefracted());
            } else {
              const glm::vec2 randomAngles = getRandomAngles();
              const float probabilityNotToTerminateRay = 0.5;

              if( !shouldTerminateRay(randomAngles.x, probabilityNotToTerminateRay) ) {
                const glm::vec3 normal = intersection.first->getNormal(intersection.second);
                const glm::vec3 direction = ray->getDirection();

                const glm::vec2 normalAngles = {std::acos(normal.z), std::atan2(normal.y, normal.x)};
                const glm::vec2 directionAngles = {std::acos(direction.z), std::atan2(direction.y, direction.x)};

                std::cout << "Normal: " << normal.x << " " << normal.y << " " << normal.z << std::endl;
                glm::vec3 vec1{1.0f, 0.0f, 0.0f};
                glm::vec3 vec2{-1.0f, 0.0f, 0.0f};
                glm::vec3 vec3{0.0f, 1.0f, 0.0f};
                glm::vec3 vec4{0.0f, -1.0f, 0.0f};
                glm::vec3 vec5{0.0f, 0.0f, 1.0f};
                glm::vec3 vec6{0.0f, 0.0f, -1.0f};
                glm::vec2 angles1 = {std::acos(vec1.z), std::atan2(vec1.y, vec1.x)};
                glm::vec2 angles2 = {std::acos(vec2.z), std::atan2(vec2.y, vec2.x)};
                glm::vec2 angles3 = {std::acos(vec3.z), std::atan2(vec3.y, vec3.x)};
                glm::vec2 angles4 = {std::acos(vec4.z), std::atan2(vec4.y, vec4.x)};
                glm::vec2 angles5 = {std::acos(vec5.z), std::atan2(vec5.y, vec5.x)};
                glm::vec2 angles6 = {std::acos(vec6.z), std::atan2(vec6.y, vec6.x)};

                // std::cout << "angles1: " << angles1.x*(180.0f / M_PI) << " " << angles1.y*(180.0f / M_PI) << std::endl;
                // std::cout << "angles2: " << angles2.x*(180.0f / M_PI) << " " << angles2.y*(180.0f / M_PI) << std::endl;
                // std::cout << "angles3: " << angles3.x*(180.0f / M_PI) << " " << angles3.y*(180.0f / M_PI) << std::endl;
                // std::cout << "angles4: " << angles4.x*(180.0f / M_PI) << " " << angles4.y*(180.0f / M_PI) << std::endl;
                // std::cout << "angles5: " << angles5.x*(180.0f / M_PI) << " " << angles5.y*(180.0f / M_PI) << std::endl;
                // std::cout << "angles6: " << angles6.x*(180.0f / M_PI) << " " << angles6.y*(180.0f / M_PI) << std::endl;



                const glm::vec2 incomingAngles = normalAngles + directionAngles;
                const glm::vec2 outgoingAngles = normalAngles + randomAngles;

                std::cout << "normalAngles: " << normalAngles.x*(180.0f / M_PI) << " " << normalAngles.y*(180.0f / M_PI) << std::endl;
                std::cout << "randomAngles: " << randomAngles.x*(180.0f / M_PI) << " " << randomAngles.y*(180.0f / M_PI) << std::endl;
                std::cout << "directionAngles: " << directionAngles.x*(180.0f / M_PI) << " " << directionAngles.y*(180.0f / M_PI) << std::endl;

                std::cout << "incomingAngles: " << incomingAngles.x*(180.0f / M_PI) << " " << incomingAngles.y*(180.0f / M_PI) << std::endl;
                std::cout << "outgoingAngles: " << outgoingAngles.x*(180.0f / M_PI) << " " << outgoingAngles.y*(180.0f / M_PI) << std::endl;

                const glm::vec3 reflection{std::sin(outgoingAngles.x)*std::cos(outgoingAngles.y),
                                           std::sin(outgoingAngles.x)*std::sin(outgoingAngles.y),
                                           std::cos(outgoingAngles.x)};
                const float importance = node->getImportance();

                const float brdf = dynamic_cast<OpaqueObject*>(intersection.first)->computeBrdf(intersection.second, incomingAngles, outgoingAngles);

                node->setReflected(new Node{new Ray{intersection.second, reflection}, importance * brdf});
                std::cout << "Origin: " << intersection.second.x << " " << intersection.second.y << " " << intersection.second.z << std::endl;
                std::cout << "Direction: " << reflection.x << " " << reflection.y << " " << reflection.z << std::endl;
                int a;
                std::cin >> a;

                traverse(node->getReflected());
              }
            }
          }

        };

        traverse(root);



        int red = 0;
        int green = 0;
        int blue = 0;
        int alpha = 0;


        image[4 * width * y + 4 * x + 0] = red;
        image[4 * width * y + 4 * x + 1] = green;
        image[4 * width * y + 4 * x + 2] = blue;
        image[4 * width * y + 4 * x + 3] = alpha;
    }
  }


  outputImage(file, image, width, height);




  for(unsigned int i=0; i<rays.size(); i++) {
    delete rays[i];
  }




  auto endTime = std::chrono::high_resolution_clock::now();
  unsigned int duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  std::cout << "DONE: " << duration << " ms" << std::endl;

  return 0;
}
