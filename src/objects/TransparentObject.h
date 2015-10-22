#ifndef TRANSPARENT_OBJECT_H
#define TRANSPARENT_OBJECT_H

#include "Object.h"
#include "meshes/Mesh.h"

class TransparentObject : public Object{
public:
  TransparentObject(Mesh* mesh, const float refractionIndex, const float transparancy = 0.5);
  virtual ~TransparentObject();

  float getRefractionIndex() const;
  float getTransparancy() const;

private:
  float refractionIndex_;
  float transparancy_;

};

#endif
