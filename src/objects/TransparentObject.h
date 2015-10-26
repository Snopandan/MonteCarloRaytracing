#ifndef TRANSPARENT_OBJECT_H
#define TRANSPARENT_OBJECT_H

#include "Object.h"
#include "meshes/Mesh.h"

class TransparentObject : public Object{
public:
  TransparentObject(const std::string& name, Mesh* mesh, const float refractionIndex, const float transparancy = 0.5, const glm::vec3& intensity = glm::vec3{1.0f, 1.0f, 1.0f});
  virtual ~TransparentObject();

  float getRefractionIndex() const;
  float getTransparancy() const;

private:
  float refractionIndex_;
  float transparancy_;

};

#endif
