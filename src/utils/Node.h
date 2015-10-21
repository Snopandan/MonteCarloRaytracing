#ifndef NODE_H
#define NODE_H

#include "Ray.h"

class Node {
public:
  Node(Ray* ray, const float importance, Node* reflected = nullptr, Node* refracted = nullptr);
  virtual ~Node();

  void setReflected(Node* reflected);
  void setRefracted(Node* refracted);

  void addIntensity(const glm::vec3& intensity);
  void setIntensity(const glm::vec3& intensity);

  Ray* getRay() const { return ray_; }
  Node* getReflected() const { return reflected_; }
  Node* getRefracted() const { return refracted_; }
  float getImportance() const { return importance_; }
  glm::vec3 getIntensity() const { return intensity_; }


private:
  Ray* ray_;
  Node* reflected_;
  Node* refracted_;
  float importance_;
  glm::vec3 intensity_;
};
#endif
