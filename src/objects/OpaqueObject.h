#ifndef OPAQUE_OBJECT_H
#define OPAQUE_OBJECT_H

#include "Object.h"
#include "meshes/Mesh.h"

class OpaqueObject : public Object{
public:
  OpaqueObject(Mesh* mesh);
  virtual ~OpaqueObject();


private:

};
#endif
