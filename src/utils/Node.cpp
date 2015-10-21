#include "Node.h"

Node::Node(Ray* ray, const float importance, Node* reflected, Node* refracted)
: ray_(ray)
, reflected_(reflected)
, refracted_(refracted)
, importance_(importance)
, intensity_{glm::vec3{0.0f, 0.0f, 0.0f}}
{

}

Node::~Node() {

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

void Node::addIntensity(const glm::vec3& intensity) {
  intensity_ += intensity;
}
