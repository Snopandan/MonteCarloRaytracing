#include "Node.h"

Node::Node(Ray* ray, const float importance, Object* lastIntersectedObject, const float refractionIndex, const bool transmitted)
: ray_(ray)
, importance_(importance)
, reflected_(nullptr)
, refracted_(nullptr)
, lastIntersectedObject_{lastIntersectedObject}
, refractionIndex_(refractionIndex)
, intensity_{glm::vec3{0.0f, 0.0f, 0.0f}}
, transmitted_{transmitted}
{

}

Node::~Node() {
  delete ray_;
}

void Node::setReflected(Node* reflected) {
  reflected_ = reflected;
}

void Node::setRefracted(Node* refracted) {
  refracted_ = refracted;
}

void Node::setIntensity(const glm::vec3& intensity) {
  intensity_ = intensity;
}

void Node::setRefractionIndex(const float refractionIndex) {
  refractionIndex_ = refractionIndex;
}

void Node::addIntensity(const glm::vec3& intensity) {
  intensity_ += intensity;
}
