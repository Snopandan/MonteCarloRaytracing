#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <thread>
#include <functional>
#include <stdexcept>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "utils/lodepng.h"

#include "Camera.h"
#include "Scene.h"
#include "Ray.h"
#include "objects/meshes/SphereMesh.h"
#include "objects/meshes/BoxMesh.h"
#include "objects/meshes/BoundingBoxMesh.h"
#include "objects/meshes/TriangleMesh.h"
#include "objects/meshes/PlaneMesh.h"
#include "objects/meshes/OrtPlaneMesh.h"
#include "objects/OpaqueObject.h"
#include "objects/TransparentObject.h"
#include "objects/brdfs/BrdfLambertian.h"


#include "thread/ThreadPool.h"
#include "thread/WorkItem.h"

#include "utils/Node.h"
#include "utils/random.h"
#include "utils/math.h"

#include "utils/lightning.h"



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

  const std::vector<Ray*> rays = camera.getRays();





  Scene scene;

  OpaqueObject* boundingBox = new OpaqueObject{new BoundingBoxMesh{glm::vec2{-10, 10}, glm::vec2{-10, 10}, glm::vec2{-10, 10}},
    new BrdfLambertian{0.5f}};
  scene.add(boundingBox);

  // float box1XTrans = 2.5;
  // float box1YTrans = 0.0;
  // float box1Width = 0.9;
  // OpaqueObject* box1 = new OpaqueObject{new BoxMesh{glm::vec2{-box1Width, box1Width} + glm::vec2{box1XTrans, box1XTrans},
  //                                                   glm::vec2{-box1Width, box1Width} + glm::vec2{box1YTrans, box1YTrans},
  //                                                   glm::vec2{1.1, 1.2}},
  //                                       new BrdfLambertian{0.5f},
  //                                       true,
  //                                       glm::vec3{0.0f, 1.0f, 0.0f}};
  // scene.add(box1);

  // OpaqueObject* sphere1 = new OpaqueObject{new SphereMesh{glm::vec3{0.0f, 0.0f, 0.0f}, 10.0f},
  //   new BrdfLambertian{1.0f}};
  // scene.add(sphere1);


  // std::vector<glm::vec3> verticies;
  // verticies.push_back(glm::vec3{-4, 0, 7});
  // verticies.push_back(glm::vec3{8, 0, 7});
  // verticies.push_back(glm::vec3{0, 9, 7});
  // OpaqueObject* triangle1 = new OpaqueObject{new TriangleMesh{verticies}};
  // scene.add(triangle1);

  // OpaqueObject* plane1 = new OpaqueObject{new PlaneMesh{glm::vec3{-8, 6, 5.5}, glm::vec3{-8, 6, 10}, glm::vec3{4, 6, 10} }};
  // scene.add(plane1)threadPool; 

  // glm::vec3 one{5.0f, 2.0f, 5.0f};
  // glm::vec3 two{5.0f, 0.0f, 5.0f};
  // glm::vec3 three{-5.0f, 0.0f, 5.0f};
  // glm::vec3 edge1 = glm::normalize(one-two);
  // glm::vec3 edge2 = glm::normalize(three-two);
  // glm::vec3 norm = -glm::normalize(glm::cross(edge1, edge2));
  // std::cout << "norm: " << norm.x << " " << norm.y << " " << norm.z << std::endl;


  OpaqueObject* plane2 = new OpaqueObject{new OrtPlaneMesh{glm::vec3{2, 2, 5},    // upperLeftCorner
                                                           glm::vec3{2, -2, 5},   // lowerLeftCorner 
                                                           glm::vec3{-2, -2, 5}}, // lowerRightCorner
                                          new BrdfLambertian{0.5f},
                                          true,
                                          glm::vec3{1.0f, 0.0f, 0.0f}};
  scene.add(plane2);

  OpaqueObject* plane3 = new OpaqueObject{new OrtPlaneMesh{glm::vec3{9, 2, 6 },    // upperLeftCorner
                                                           glm::vec3{9, 0, 6},   // lowerLeftCorner 
                                                           glm::vec3{9, 0, 4}}, // lowerRightCorner
                                          new BrdfLambertian{0.5f},
                                          true,
                                          glm::vec3{0.0f, 0.0f, 1.0f}};
  scene.add(plane3);


  scene.complete();

  std::vector<unsigned char> image;
  image.resize(width * height * 4);
  std::fill(image.begin(), image.end(), 0);


  ThreadPool threadPool;

  // threadPool.setNumberOfWorkers(0);

  // Construct importance tree
  Node* importanceTree[width][height];

  for(unsigned x = 0; x < width; x++) {
    WorkItem* workItem = new WorkItem([&image, &importanceTree, &scene, &rays, x]() {

      for(unsigned y = 0; y < height; y++) {

        const float rootImportance = 1.0f;
        Node* root = new Node{rays[width * y + x], rootImportance};

        std::function<void(Node*)> traverse = [&scene, &traverse, &root](Node* node) {
          const Ray* ray = node->getRay();
          const std::pair<Object*, glm::vec3> intersection = scene.intersect(ray);

          if( intersection.first == nullptr) {
            const glm::vec3 direction = ray->getDirection();
            const glm::vec3 origin = ray->getOrigin();
            throw std::invalid_argument{"No intersection found."};
          }

          if( intersection.first->isLight() ) {
            const glm::vec3 origin = ray->getOrigin();
            const glm::vec3 direction = ray->getDirection();
            const glm::vec3 normal = intersection.first->getNormal(intersection.second);
            const glm::vec3 viewDirection = glm::normalize(origin);

            // TODO: Material properties
            const float ka = 0.3f;
            const float kd = 0.7f;
            const float ks = 0.2f;
            const glm::vec3 diffuseColor = glm::vec3(0.9f, 0.9f, 0.9f);;
            const glm::vec3 ambientColor = diffuseColor;
            const glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
            const float specularity = 5;

            glm::vec3 color = localLightning(origin, 
                                             direction, 
                                             normal, 
                                             viewDirection,
                                             ka,
                                             kd,
                                             ks,
                                             ambientColor,
                                             diffuseColor,
                                             specularColor,
                                             specularity);

            // TODO: Do something with color!
            node->setIntensity(color * intersection.first->getIntensity());
          }

          if( intersection.first != nullptr && !intersection.first->isLight() ) {
            if( intersection.first->isTransparent() ) {
              const glm::vec3 direction = ray->getDirection();
              const glm::vec3 normal = intersection.first->getNormal(intersection.second);

              const glm::vec3 reflection = glm::reflect(direction, normal);

              const float materialRefractionIndex = dynamic_cast<TransparentObject*>(intersection.first)->getRefractionIndex();

              Object* lastIntersectedObject = node->getLastIntersectedObject();

              const float nodeRefractionIndex = node->getRefractionIndex();

              float n1 = nodeRefractionIndex;
              float n2 = materialRefractionIndex;

              if( lastIntersectedObject == intersection.first && nodeRefractionIndex == materialRefractionIndex ) {
                n1 = materialRefractionIndex;
                n2 = 1.0f;
              } 

              const float refractionIndexRatio = n1 / n2; 
              const glm::vec3 refraction = glm::refract(direction, normal, refractionIndexRatio);

              const float importance = node->getImportance();
              const float transparency = dynamic_cast<TransparentObject*>(intersection.first)->getTransparancy();

              const glm::vec3 newReflectedOrigin = intersection.second + normal * getEpsilon() - direction * getEpsilon();
              const glm::vec3 newRefractedOrigin = intersection.second - normal * getEpsilon() + direction * getEpsilon();

              // TODO: Compute Fresnel in order to give the right porportions to the reflected and refracted part!
              node->setReflected(new Node{new Ray{newReflectedOrigin, reflection}, importance * (1 - transparency), intersection.first, n2});
              node->setRefracted(new Node{new Ray{newRefractedOrigin, refraction}, importance * transparency, intersection.first, n2});

              traverse(node->getReflected());
              traverse(node->getRefracted());
            } else {
              const glm::vec2 randomAngles = getRandomAngles();
              const float probabilityNotToTerminateRay = 0.5;

              if( !shouldTerminateRay(randomAngles.x, probabilityNotToTerminateRay) || node == root ) {
                const glm::vec3 normal = intersection.first->getNormal(intersection.second);
                const glm::vec3 direction = ray->getDirection();

                const glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
                const glm::mat4 rotation = glm::orientation(normal, up);

                const glm::vec4 hemisphereIncomingDirectionFlipped = -(rotation * glm::vec4(direction.x, direction.y, direction.z, 1.0f));

                const glm::vec2 directionAngles = {std::acos(hemisphereIncomingDirectionFlipped.z), std::atan2(hemisphereIncomingDirectionFlipped.y, hemisphereIncomingDirectionFlipped.x)};

                const glm::vec2 incomingAngles = directionAngles;
                const glm::vec2 outgoingAngles = randomAngles;

                assert( randomAngles.x >= 0.0f && randomAngles.x <= 2.0 * M_PI );
                assert( randomAngles.y >= 0.0f && randomAngles.y <= M_PI / 2.0 );

                const glm::vec3 reflection{std::sin(outgoingAngles.y)*std::cos(outgoingAngles.x),
                                           std::sin(outgoingAngles.y)*std::sin(outgoingAngles.x),
                                           std::cos(outgoingAngles.y)};

                const float importance = node->getImportance();

                const float brdf = dynamic_cast<OpaqueObject*>(intersection.first)->computeBrdf(intersection.second, incomingAngles, outgoingAngles);

                const glm::vec3 newReflectedOrigin = intersection.second + normal * getEpsilon() - direction * getEpsilon();

                node->setReflected(new Node{new Ray{newReflectedOrigin, reflection}, importance * brdf, intersection.first, node->getRefractionIndex()});

                traverse(node->getReflected());

                node->setIntensity(importance * scene.castShadowRays(newReflectedOrigin, 1));
                // node->addIntensity(importance * node->getReflected()->getIntensity());

              }
            }
          }

        };

        importanceTree[x][y] = root;
        traverse(root);

        const glm::vec3 color = importanceTree[x][y]->getIntensity();
        int red = color.r * 100;
        int green = color.g * 100;
        int blue = color.b * 100;
        int alpha = 255;
        image[4 * width * y + 4 * x + 0] = red;
        image[4 * width * y + 4 * x + 1] = green;
        image[4 * width * y + 4 * x + 2] = blue;
        image[4 * width * y + 4 * x + 3] = alpha;
        
      }

    });
    threadPool.add(workItem);
  }
  threadPool.wait();



  // // Compute pixels
  // for(unsigned x = 0; x < width; x++) {
  //   WorkItem* workItem = new WorkItem([&, x]() {

  //     for(unsigned y = 0; y < height; y++) {

  //       std::function<void(Node*)> traverse = [&scene, &traverse](Node* node) {
  //         Node* reflected = node->getReflected();
  //         Node* refracted = node->getRefracted();

  //         if( reflected != nullptr ) {
  //           traverse(reflected);
  //           node->addIntensity(node->getImportance() * reflected->getIntensity());
  //         }

  //         if( refracted != nullptr ) {
  //           traverse(refracted);
  //           node->addIntensity(node->getImportance() * refracted->getIntensity());
  //         }

  //       };

  //       traverse(importanceTree[x][y]);

  //       const glm::vec3 color = importanceTree[x][y]->getIntensity();
  //       // std::cout << "Color: " << color.r << " " << color.g << " " << color.b << std::endl;
  //       // std::string line;
  //       // std::getline(std::cin, line);

  //       int red = color.r * 100;
  //       int green = color.g * 100;
  //       int blue = color.b * 100;
  //       int alpha = 255;

  //       image[4 * width * y + 4 * x + 0] = red;
  //       image[4 * width * y + 4 * x + 1] = green;
  //       image[4 * width * y + 4 * x + 2] = blue;
  //       image[4 * width * y + 4 * x + 3] = alpha;

  //     }

  //   });
  //   threadPool.add(workItem);
  // }
  // threadPool.wait();



  outputImage(file, image, width, height);



  // CLEANUP! Rays?, ImportanceTree?, Scene?


  auto endTime = std::chrono::high_resolution_clock::now();
  unsigned int duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  std::cout << "DONE: " << duration << " ms" << std::endl;

  return 0;
}
