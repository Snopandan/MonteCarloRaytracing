#ifndef TRANSPARENT_OBJECT_H
#define TRANSPARENT_OBJECT_H

#include "Object.h"
#include "meshes/Mesh.h"

class TransparentObject : public Object{
public:
  TransparentObject(Mesh* mesh);
  virtual ~TransparentObject();

private:

};
#endif
