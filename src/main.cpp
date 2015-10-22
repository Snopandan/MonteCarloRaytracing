#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <thread>
#include <functional>
#include <stdexcept>
#include <cmath>
#include <iomanip>
#include <mutex>
#include <sstream>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "Camera.h"
#include "Scene.h"
#include "Ray.h"
#include "objects/meshes/SphereMesh.h"
#include "objects/meshes/BoxMesh.h"
#include "objects/meshes/BoundingBoxMesh.h"
#include "objects/meshes/OrtPlaneMesh.h"
#include "objects/OpaqueObject.h"
#include "objects/TransparentObject.h"
#include "objects/brdfs/BrdfLambertian.h"

#include "thread/ThreadPool.h"
#include "thread/WorkItem.h"

#include "utils/Node.h"
#include "utils/random.h"
#include "utils/math.h"
#include "utils/image.h"
#include "utils/lightning.h"


Scene createScene() {
  Scene scene;

  OpaqueObject* boundingBox = new OpaqueObject{new BoundingBoxMesh{glm::vec2{-10, 10}, glm::vec2{-10, 10}, glm::vec2{-10, 10}},
                                               new BrdfLambertian{0.5f}};
  scene.add(boundingBox);

  OpaqueObject* lightPlane1 = new OpaqueObject{new OrtPlaneMesh{glm::vec3{2, 2, 5},    // upperLeftCorner
                                                                glm::vec3{2, -2, 5},   // lowerLeftCorner 
                                                                glm::vec3{-2, -2, 5}}, // lowerRightCorner
                                               new BrdfLambertian{0.5f},
                                               true,
                                               glm::vec3{1.0f, 0.0f, 0.0f}};
  scene.add(lightPlane1);

  OpaqueObject* lightPlane2 = new OpaqueObject{new OrtPlaneMesh{glm::vec3{9, 2, 6},  // upperLeftCorner
                                                                glm::vec3{9, 0, 6},  // lowerLeftCorner 
                                                                glm::vec3{9, 0, 4}}, // lowerRightCorner
                                               new BrdfLambertian{0.5f},
                                               true,
                                               glm::vec3{0.0f, 0.0f, 1.0f}};
  scene.add(lightPlane2);

  scene.complete();

  return scene;
}


int main(const int argc, const char* argv[]) {

  const auto startTime = std::chrono::high_resolution_clock::now();

  const unsigned int width = 800;
  const unsigned int height = 600;

  Camera camera{glm::ivec2{width, height},   // pixels
                glm::vec2{0.01f, 0.01f},     // pixelSize
                glm::vec3{0.0f, 0.0f, 0.0f}, // position
                1.0f};                       // viewPlaneDistance

  const std::vector<Ray*> rays = camera.getRays();

  Scene scene = createScene();

  std::vector<unsigned char> image;
  image.resize(width * height * 4);
  std::fill(image.begin(), image.end(), 0);

  ThreadPool threadPool;
  // threadPool.setNumberOfWorkers(0); // Use if you want to run the application single threaded
  std::mutex update;
  unsigned int columnCounter = 0;

  const float rootImportance = 1.0f;

  for(unsigned x = 0; x < width; x++) {
    WorkItem* workItem = new WorkItem([&update, &columnCounter, &image, &scene, &rays, &rootImportance, x]() {

      for(unsigned y = 0; y < height; y++) {

        Node* root = new Node{rays[width * y + x], rootImportance};

        const std::function<void(Node*)> traverse = [&scene, &traverse, &root](Node* node) {
          const Ray* ray = node->getRay();
          const std::pair<Object*, glm::vec3> intersection = scene.intersect(ray);

          if( intersection.first == nullptr ) { // No intersection found
            throw std::invalid_argument{"No intersection found."};

          } else if( intersection.first->isLight() ) { // If intersecting object is a light source

            const glm::vec3 origin = ray->getOrigin();
            const glm::vec3 direction = ray->getDirection();
            const glm::vec3 normal = intersection.first->getNormal(intersection.second);
            const glm::vec3 viewDirection = glm::normalize(origin); // TODO?: camera not always in origin

            // TODO: Material properties
            const float ka = 0.3f;
            const float kd = 0.7f;
            const float ks = 0.2f;
            const glm::vec3 diffuseColor = glm::vec3(0.9f, 0.9f, 0.9f);;
            const glm::vec3 ambientColor = diffuseColor;
            const glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
            const float specularity = 5.0f;

            const glm::vec3 color = localLightning(direction, 
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

          } else if( intersection.first->isTransparent() ) { // If intersecting object is transparent

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
              n2 = 1.0f; // Air
            } 

            const float refractionIndexRatio = n1 / n2; 
            const glm::vec3 refraction = glm::refract(direction, normal, refractionIndexRatio);

            const float importance = node->getImportance();
            const float transparency = dynamic_cast<TransparentObject*>(intersection.first)->getTransparancy();

            const glm::vec3 newReflectedOrigin = intersection.second + (normal - direction) * getEpsilon();
            const glm::vec3 newRefractedOrigin = intersection.second + (direction - normal) * getEpsilon();

            // TODO: Compute Fresnel in order to give the right porportions to the reflected and refracted part!
            node->setReflected(new Node{new Ray{newReflectedOrigin, reflection}, importance * (1.0f - transparency), intersection.first, n2});
            node->setRefracted(new Node{new Ray{newRefractedOrigin, refraction}, importance * transparency, intersection.first, n2});

            traverse(node->getReflected());
            traverse(node->getRefracted());

          } else { // If intersecting object is opaque and not a light source

            const glm::vec2 outgoingAngles = getRandomAngles();
            const float probabilityNotToTerminateRay = 0.65f;

            if( !shouldTerminateRay(outgoingAngles.y, probabilityNotToTerminateRay) || node == root ) {

              const glm::vec3 normal = intersection.first->getNormal(intersection.second);
              const glm::vec3 direction = ray->getDirection();

              const glm::mat3 rotation = computeRotationMatrix(normal);

              const glm::vec3 hemisphereIncomingDirectionFlipped = -(rotation * direction);

              const glm::vec2 incomingAngles = {std::acos(hemisphereIncomingDirectionFlipped.z), 
                                                std::atan2(hemisphereIncomingDirectionFlipped.y, hemisphereIncomingDirectionFlipped.x)};

              const glm::vec3 reflection = rotation * glm::vec3{std::sin(outgoingAngles.x) * std::cos(outgoingAngles.y),
                                                                std::sin(outgoingAngles.x) * std::sin(outgoingAngles.y),
                                                                std::cos(outgoingAngles.x)};
              const float importance = node->getImportance();

              const float brdf = dynamic_cast<OpaqueObject*>(intersection.first)->computeBrdf(intersection.second, incomingAngles, outgoingAngles);

              const glm::vec3 newReflectedOrigin = intersection.second + (normal - direction) * getEpsilon();

              node->setReflected(new Node{new Ray{newReflectedOrigin, reflection}, importance * brdf, intersection.first, node->getRefractionIndex()});

              traverse(node->getReflected());

              node->setIntensity(importance * scene.castShadowRays(newReflectedOrigin, 4));
              node->addIntensity(importance * node->getReflected()->getIntensity());

            }

          }

        };

        traverse(root);

        const glm::vec3 color = root->getIntensity();

        const int red = std::min( (int)(color.r * 100), 255);
        const int green = std::min( (int)(color.g * 100), 255);
        const int blue = std::min( (int)(color.b * 100), 255);
        const int alpha = 255;

        image[4 * width * y + 4 * x + 0] = red;
        image[4 * width * y + 4 * x + 1] = green;
        image[4 * width * y + 4 * x + 2] = blue;
        image[4 * width * y + 4 * x + 3] = alpha;
      }

      update.lock();
      std::cout << "\r" << (int)((++columnCounter / (float) width) * 100) << "%";
      std::flush(std::cout);
      update.unlock();

    });
    threadPool.add(workItem);
  }
  threadPool.wait();

  const std::string file = "test.png";
  outputImage(file, image, width, height);

  // TODO: Cleanup!

  const auto endTime = std::chrono::high_resolution_clock::now();
  const unsigned int duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  std::cout << " | " << file << " | " << duration << " ms" << std::endl;

  return 0;
}
