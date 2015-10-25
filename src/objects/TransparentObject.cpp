#include "TransparentObject.h"

TransparentObject::TransparentObject(const std::string& name, Mesh* mesh, const float refractionIndex, const float transparancy)
: Object(name, mesh, true, false)
, refractionIndex_{refractionIndex}
, transparancy_{transparancy} {

}

TransparentObject::~TransparentObject() {

}

float TransparentObject::getRefractionIndex() const {
  return refractionIndex_;
}

float TransparentObject::getTransparancy() const {
  return transparancy_;
}
