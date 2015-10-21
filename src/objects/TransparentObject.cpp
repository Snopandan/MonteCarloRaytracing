#include "TransparentObject.h"

TransparentObject::TransparentObject(Mesh* mesh, const float transparancy)
: Object(mesh, true, false), transparancy_{transparancy} {

}

TransparentObject::~TransparentObject() {

}

float TransparentObject::getTransparancy() const {
  return transparancy_;
}
