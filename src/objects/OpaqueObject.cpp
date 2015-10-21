#include "OpaqueObject.h"

OpaqueObject::OpaqueObject(Mesh* mesh, Brdf* brdf, const bool isLight)
: Object(mesh, false, isLight), brdf_(brdf) {

}

OpaqueObject::~OpaqueObject() {
  delete brdf_;
}
