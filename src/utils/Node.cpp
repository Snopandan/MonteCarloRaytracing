#include "Node.h"

Node::Node(Ray* ray, const float importance, Node* reflected, Node* refracted)
: ray_(ray), reflected_(reflected), refracted_(refracted), importance_(importance){

}

Node::~Node() {

}

void Node::setReflected(Node* reflected) {
  reflected_ = reflected;
}

void Node::setRefracted(Node* refracted) {
  refracted_ = refracted;
}
