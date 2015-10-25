#ifndef NODE_H
#define NODE_H

#include "Ray.h"
#include "objects/Object.h"

class Node {
public:
  Node(Ray* ray, const float importance, Object* lastIntersectedObject = nullptr, const float refractionIndex = 1.0f,
      const bool transmitted = false);
  virtual ~Node();

  void setReflected(Node* reflected);
  void setRefracted(Node* refracted);

  void addIntensity(const glm::vec3& intensity);
  void setIntensity(const glm::vec3& intensity);

  void setRefractionIndex(const float refractionIndex);
  float getRefractionIndex() const { return refractionIndex_; }

  Ray* getRay() const { return ray_; }
  Node* getReflected() const { return reflected_; }
  Node* getRefracted() const { return refracted_; }
  Object* getLastIntersectedObject() const { return lastIntersectedObject_; }
  float getImportance() const { return importance_; }
  glm::vec3 getIntensity() const { return intensity_; }
  bool isTransmitted() const { return transmitted_; }


private:
  Ray* ray_;
  Node* reflected_;
  Node* refracted_;
  Object* lastIntersectedObject_;
  float importance_;
  float refractionIndex_;
  glm::vec3 intensity_;
  bool transmitted_;
};
#endif
