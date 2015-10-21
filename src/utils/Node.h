#ifndef NODE_H
#define NODE_H

#include "Ray.h"

class Node {
public:
  Node(Ray* ray, const float importance, Node* reflected = nullptr, Node* refracted = nullptr);
  virtual ~Node();

  void setReflected(Node* reflected);
  void setRefracted(Node* refracted);

  Ray* getRay() const { return ray_; }
  Node* getReflected() const { return reflected_; }
  Node* getRefracted() const { return refracted_; }
  float getImportance() const { return importance_; }

private:
  Ray* ray_;
  Node* reflected_;
  Node* refracted_;
  float importance_;
};
#endif
