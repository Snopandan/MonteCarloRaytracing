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

#define GLM_FORCE_RADIANS
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

  const unsigned int width = 800;
  const unsigned int height = 600;

  Camera camera{glm::ivec2{width, height}, // pixels
                glm::vec2{0.01f, 0.01f},   // pixelSize
                glm::vec3{0, 0, 0},        // position
                1};                        // viewPlaneDistance

  const std::vector<Ray*> rays = camera.getRays();



  Scene scene = createScene();

  // glm::vec3 newN{1.0f, 0.0f, 0.0f};
  // glm::vec3 oldN{0.0f, 1.0f, 0.0f};
  // glm::vec3 vec1{0.0f, 0.0f, 1.0f};

  // glm::mat3 mata = glm::outerProduct(newN, oldN);

  // glm::mat4 mata2;

  // mata2[0][0] = mata[0][0];
  // mata2[0][1] = mata[0][1];
  // mata2[0][2] = mata[0][2];
  // mata2[0][3] = 0;
  // mata2[1][0] = mata[1][0];
  // mata2[1][1] = mata[1][1];
  // mata2[1][2] = mata[1][2];
  // mata2[1][3] = 0;
  // mata2[2][0] = mata[2][0];
  // mata2[2][1] = mata[2][1];
  // mata2[2][2] = mata[2][2];
  // mata2[2][3] = 0;
  // mata2[3][0] = 0;
  // mata2[3][1] = 0;
  // mata2[3][2] = 0;
  // mata2[3][3] = 1;

  // // glm::vec4 tri =  mata2 * glm::vec4(oldN.x, oldN.y, oldN.z, 1.0);
  // glm::vec4 tri =  mata2 * glm::vec4(vec1.x, vec1.y, vec1.z, 1.0);
  // glm::vec3 fin =  glm::vec3(tri.x, tri.y, tri.z);
  // std::cout << "fin: " << tri.x << ", " << tri.y << ", " << tri.z << std::endl;



  // glm::vec3 normal = glm::vec3{0, 0, 1};

  // int imin = 0;
  // for(int i=0; i<3; ++i) {
  //   if(std::abs(normal[i]) < std::abs(normal[imin])) {
  //     imin = i;
  //   }
  // }
      
  // glm::vec3 v2{0.0f, 0.0f, 0.0f};

  // float dt = normal[imin];

  // v2[imin] = 1.0f;
  // for(int i=0;i<3;i++) {
  //   v2[i] -= dt*normal[i];
  // }

  // glm::vec3 v3 = glm::cross(normal, v2);

  // std::cout << std::left << std::setw(16) << "normal: "  << normal.x << " " << normal.y << " " << normal.z << std::endl;
  // std::cout << std::left << std::setw(16) << "v2: " << v2.x << " " << v2.y << " " << v2.z << std::endl;
  // std::cout << std::left << std::setw(16) << "v3: " << v3.x << " " << v3.y << " " << v3.z << std::endl;

  // glm::mat4 rotation;
  // rotation[0][0] = v2.x;     rotation[0][1] = v2.y;     rotation[0][2] = v2.z;     rotation[0][3] = 0.0f;
  // rotation[1][0] = v3.x;     rotation[1][1] = v3.y;     rotation[1][2] = v3.z;     rotation[1][3] = 0.0f;
  // rotation[2][0] = normal.x; rotation[2][1] = normal.y; rotation[2][2] = normal.z; rotation[2][3] = 0.0f;
  // rotation[3][0] = 0.0f;     rotation[3][1] = 0.0f;     rotation[3][2] = 0.0f;     rotation[3][3] = 1.0;

  // // std::cout << "Rotation:" << std::endl;
  // // std::cout << std::left << std::setw(10) << rotation[0][0] << std::left << std::setw(10) << rotation[0][1] << std::left << std::setw(10) << rotation[0][2] << std::left << std::setw(10) << rotation[0][3] << std::endl;
  // // std::cout << std::left << std::setw(10) << rotation[1][0] << std::left << std::setw(10) << rotation[1][1] << std::left << std::setw(10) << rotation[1][2] << std::left << std::setw(10) << rotation[1][3] << std::endl;
  // // std::cout << std::left << std::setw(10) << rotation[2][0] << std::left << std::setw(10) << rotation[2][1] << std::left << std::setw(10) << rotation[2][2] << std::left << std::setw(10) << rotation[2][3] << std::endl;
  // // std::cout << std::left << std::setw(10) << rotation[3][0] << std::left << std::setw(10) << rotation[3][1] << std::left << std::setw(10) << rotation[3][2] << std::left << std::setw(10) << rotation[3][3] << std::endl;

  // glm::mat4 rotation2;
  // rotation2[0][0] = v2.x;     rotation2[0][1] = v3.x;     rotation2[0][2] = normal.x;     rotation2[0][3] = 0.0f;
  // rotation2[1][0] = v2.y;     rotation2[1][1] = v3.y;     rotation2[1][2] = normal.y;     rotation2[1][3] = 0.0f;
  // rotation2[2][0] = v2.z;     rotation2[2][1] = v3.z;     rotation2[2][2] = normal.z;     rotation2[2][3] = 0.0f;
  // rotation2[3][0] = 0.0f;     rotation2[3][1] = 0.0f;     rotation2[3][2] = 0.0f;         rotation2[3][3] = 1.0;


  // glm::vec3 vecToRotate{0.0f, -1.0f, 0.0f};
  // std::cout << std::left << std::setw(16) << "vecToRotate: " << vecToRotate.x << " " << vecToRotate.y << " " << vecToRotate.z << std::endl;

  // glm::vec4 answer = rotation * glm::vec4(vecToRotate.x, vecToRotate.y, vecToRotate.z, 1.0f);
  // glm::vec3 final = glm::vec3(answer.x, answer.y, answer.z);
  // std::cout << std::left << std::setw(16) << "final: " << final.x << " " << final.y << " " << final.z << std::endl;

  // glm::vec4 answer2 = rotation2 * glm::vec4(vecToRotate.x, vecToRotate.y, vecToRotate.z, 1.0f);
  // glm::vec3 final2 = glm::vec3(answer2.x, answer2.y, answer2.z);
  // std::cout << std::left << std::setw(16) << "final2: " << final2.x << " " << final2.y << " " << final2.z << std::endl;


  // return 0;

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

          // std::cout << "Ray origin: " << ray->getOrigin().x << " " << ray->getOrigin().y << " " << ray->getOrigin().z << std::endl;
          // std::cout << "Ray direction: " << ray->getDirection().x << " " << ray->getDirection().y << " " << ray->getDirection().z << std::endl;
          // std::cout << "Intersection: " << intersection.second.x << " " << intersection.second.y << " " << intersection.second.z << std::endl;
          // std::string line;
          // std::getline(std::cin, line);


          if( intersection.first == nullptr) { // No intersection found
            throw std::invalid_argument{"No intersection found."};

          } else if( intersection.first->isLight() ) { // If intersecting object is a light source

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
            const float specularity = 5.0f;

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
              n2 = 1.0f;
            } 

            const float refractionIndexRatio = n1 / n2; 
            const glm::vec3 refraction = glm::refract(direction, normal, refractionIndexRatio);

            const float importance = node->getImportance();
            const float transparency = dynamic_cast<TransparentObject*>(intersection.first)->getTransparancy();

            const glm::vec3 newReflectedOrigin = intersection.second + normal * getEpsilon() - direction * getEpsilon();
            const glm::vec3 newRefractedOrigin = intersection.second - normal * getEpsilon() + direction * getEpsilon();

            // TODO: Compute Fresnel in order to give the right porportions to the reflected and refracted part!
            node->setReflected(new Node{new Ray{newReflectedOrigin, reflection}, importance * (1.0f - transparency), intersection.first, n2});
            node->setRefracted(new Node{new Ray{newRefractedOrigin, refraction}, importance * transparency, intersection.first, n2});

            traverse(node->getReflected());
            traverse(node->getRefracted());

          } else { // If intersecting object is opaque and not a light source

            const glm::vec2 randomAngles = getRandomAngles();
            // std::cout << "randomAngles: " << randomAngles.x  * (180.0f/M_PI) << " " << randomAngles.y * (180.0f/M_PI) << std::endl;
            const float probabilityNotToTerminateRay = 0.65f;

            if( !shouldTerminateRay(randomAngles.y, probabilityNotToTerminateRay) || node == root ) {

              // if( intersection.second.z < 0 )
               // std::cout << intersection.second.x << " " << intersection.second.y << " " << intersection.second.z << std::endl;

              const glm::vec3 normal = intersection.first->getNormal(intersection.second);
              const glm::vec3 direction = ray->getDirection();

              // const glm::vec3 up = glm::normalize(glm::cross(-direction, normal));
              // glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
              // const glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);
              // const glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

              // glm::vec3 normalCrossUp = glm::cross(normal, up);
              // glm::vec3 rotationAxis = glm::normalize(normalCrossUp);
              // glm::mat4 rotation = glm::rotate(std::asin(glm::length(normalCrossUp)), rotationAxis);

                // glm::vec3 normal = glm::vec3{0, 0, 1};

              int imin = 0;
              for(int i=0; i<3; ++i) {
                if(std::abs(normal[i]) < std::abs(normal[imin])) {
                  imin = i;
                }
              }
                  
              glm::vec3 v2{0.0f, 0.0f, 0.0f};

              float dt = normal[imin];

              v2[imin] = 1.0f;
              for(int i=0;i<3;i++) {
                v2[i] -= dt*normal[i];
              }

              glm::vec3 v3 = glm::cross(normal, v2);

              glm::mat4 rotation;
              rotation[0][0] = v2.x;     rotation[0][1] = v2.y;     rotation[0][2] = v2.z;     rotation[0][3] = 0.0f;
              rotation[1][0] = v3.x;     rotation[1][1] = v3.y;     rotation[1][2] = v3.z;     rotation[1][3] = 0.0f;
              rotation[2][0] = normal.x; rotation[2][1] = normal.y; rotation[2][2] = normal.z; rotation[2][3] = 0.0f;
              rotation[3][0] = 0.0f;     rotation[3][1] = 0.0f;     rotation[3][2] = 0.0f;     rotation[3][3] = 1.0;


              // glm::mat4 rotation = glm::orientation(normal, up);
              // glm::mat4 inverseRotation = glm::inverse(rotation);
               // glm::lookAt(position_, position_ + direction_, up_);

              // glm::mat4 rotation = glm::orientation(normal, up);

              // rotation[3][0] = 0.0;
              // rotation[3][1] = 0.0;
              // rotation[3][2] = 0.0;

              // rotation[0][3] = 0.0;
              // rotation[1][3] = 0.0;
              // rotation[2][3] = 0.0;

              // if( normal.y >= 0 ) {
              //   rotation = glm::orientation(normal, up);
              // } else {
              //   rotation = glm::orientation(normal, down);
              // }

              const glm::vec4 hemisphereIncomingDirectionFlipped = -(rotation * glm::vec4(direction.x, direction.y, direction.z, 1.0f));

              const glm::vec2 directionAngles = {std::acos(hemisphereIncomingDirectionFlipped.z), std::atan2(hemisphereIncomingDirectionFlipped.y, hemisphereIncomingDirectionFlipped.x)};

              const glm::vec2 incomingAngles = directionAngles;
              const glm::vec2 outgoingAngles = randomAngles;

              // const float normInc = std::acos(normal.z);
              // const float normX = std::atan2(normal.y, normal.z);

              const glm::vec2 normalAngles = {std::acos(normal.z), 
                                              std::atan2(normal.y, normal.x)};

              const glm::vec2 directionAngles2 = {std::acos(direction.z), 
                                              std::atan2(direction.y, direction.x)};

              // std::cout << "normInc: " << normInc * (180.0f/M_PI) << std::endl;
              // std::cout << "normX: " << normX * (180.0f/M_PI) << std::endl;
              // glm::vec2 outgoingAngles = randomAngles - normalAngles;
              glm::vec2 outgoingAngles2 = normalAngles + randomAngles - glm::vec2{M_PI/2.0f, 0};

              // std::cout << "randomAngles: " << (randomAngles.x * (180.0f/(float)M_PI)) << " " << (randomAngles.y * (180.0f/(float)M_PI))<< std::endl;
              // std::cout << "normalAngles: " << (normalAngles.x * (180.0f/(float)M_PI)) << " " << (normalAngles.y * (180.0f/(float)M_PI))<< std::endl;
              // std::cout << "outgoingAngles2: " << (outgoingAngles2.x * (180.0f/(float)M_PI)) << " " << (outgoingAngles2.y * (180.0f/(float)M_PI)) << std::endl;

                                          // INC, HORIZONTAL
              glm::vec4 reflection1{std::sin(randomAngles.x) * std::cos(randomAngles.y),
                                    std::sin(randomAngles.x) * std::sin(randomAngles.y),
                                    std::cos(randomAngles.x), 1.0f};

              // std::cout << "RandomVector: " << reflection1.x << " " << reflection1.y << " " << reflection1.z << std::endl;

              glm::vec4 reflection2 = rotation * reflection1;
              glm::vec3 reflection = glm::vec3(reflection2.x, reflection2.y, reflection2.z);



              // glm::vec3 n = normal;
              // // glm::vec3 n = glm::vec3(0,1,0);
              // glm::vec2 rota = randomAngles;
              // // rota = glm::vec2{M_PI/2.0f, M_PI/2.0f};
              // glm::vec3 reflection = glm::rotateX(n, rota.x * 180.0f / (float)M_PI);
              // std::cout << "reflectionX: " << reflection.x << " " << reflection.y << " " << reflection.z << std::endl;
              // reflection = glm::rotateY(reflection, rota.y);
              // std::cout << "reflectionY: " << reflection.x << " " << reflection.y << " " << reflection.z << std::endl;



              // glm::vec3 n = normal;
              // // glm::vec3 n = glm::vec3(0,1,0);
              // glm::vec2 rota = randomAngles;
              // // rota = glm::vec2{M_PI/2.0f, M_PI/2.0f};
              // glm::vec3 reflection = glm::rotate(normal, rota.x * 180.0f / (float)M_PI, glm::vec3(1.0f, 0.0f, 0.0f));
              // // std::cout << "reflection: " << reflection.x << " " << reflection.y << " " << reflection.z << std::endl;
              // reflection = glm::rotate(reflection, rota.y * 180.0f / (float)M_PI, glm::vec3(0.0f, 1.0f, 0.0f));
              // // std::cout << "reflection: " << reflection.x << " " << reflection.y << " " << reflection.z << std::endl;
              



              // const glm::vec4 inv = (glm::vec4(reflection2.x, reflection2.y, reflection2.z, 1.0f) * inverseRotation);

              // const glm::vec3 reflection = glm::vec3{inv.x, inv.y, inv.z};

              // std::cout << "rotation: " << std::endl;
              // std::cout << rotation[0][0] << " " << rotation[0][1] << " " << rotation[0][2] << " " << rotation[0][3] << std::endl;
              // std::cout << rotation[1][0] << " " << rotation[1][1] << " " << rotation[1][2] << " " << rotation[1][3] << std::endl;
              // std::cout << rotation[2][0] << " " << rotation[2][1] << " " << rotation[2][2] << " " << rotation[2][3] << std::endl;
              // std::cout << rotation[3][0] << " " << rotation[3][1] << " " << rotation[3][2] << " " << rotation[3][3] << std::endl;

              // std::cout << "Normal: " << normal.x << " " << normal.y << " " << normal.z << std::endl;
              // std::cout << "randomRef: " << randomRef.x << " " << randomRef.y << " " << randomRef.z << std::endl;
              // // std::cout << "reflection2: " << reflection2.x << " " << reflection2.y << " " << reflection2.z << std::endl;
              // const glm::vec4 reflection3 = (rotation * glm::vec4(reflection2.x, reflection2.y, reflection2.z, 1.0f));
              // const glm::vec3 reflection = glm::vec3(reflection3.x, reflection3.y, reflection3.z);
              // std::cout << "direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
              // std::cout << "reflection: " << reflection.x << " " << reflection.y << " " << reflection.z << std::endl;

              // const glm::vec3 trueReflection = glm::reflect(direction, normal);
              // std::cout << "trueReflection: " << trueReflection.x << " " << trueReflection.y << " " << trueReflection.z << std::endl;

              // std::string line;
              // std::getline(std::cin, line);



              const float importance = node->getImportance();

              const float brdf = dynamic_cast<OpaqueObject*>(intersection.first)->computeBrdf(intersection.second, incomingAngles, outgoingAngles);

              const glm::vec3 newReflectedOrigin = intersection.second + normal * getEpsilon() - direction * getEpsilon();

              node->setReflected(new Node{new Ray{newReflectedOrigin, reflection}, importance * brdf, intersection.first, node->getRefractionIndex()});

              traverse(node->getReflected());

              node->setIntensity(importance * scene.castShadowRays(newReflectedOrigin, 4));
              node->addIntensity(importance * node->getReflected()->getIntensity());
            }

          }
          
        };

        importanceTree[x][y] = root;
        traverse(root);

        const glm::vec3 color = importanceTree[x][y]->getIntensity();

        int red = std::min( (int)(color.r * 100), 255);
        int green = std::min( (int)(color.g * 100), 255);
        int blue = std::min( (int)(color.b * 100), 255);
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


  const std::string file = "test.png";
  outputImage(file, image, width, height);



  // CLEANUP! Rays?, ImportanceTree?, Scene?


  auto endTime = std::chrono::high_resolution_clock::now();
  unsigned int duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  std::cout << "DONE: " << duration << " ms" << std::endl;

  return 0;
}
