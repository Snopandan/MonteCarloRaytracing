#ifndef OBJECT_H
#define OBJECT_H

#include <utility>
#include <stdexcept>

#include <glm/glm.hpp>

#include "Ray.h"
#include "meshes/Mesh.h"

class Object {
public:
  enum class Intersection {MISS, HIT};

  Object(const std::string& name, Mesh* mesh, const bool isTransparent = false, const bool isLight = false, const glm::vec3& intensity = glm::vec3{1.0f, 1.0f, 1.0f});
  virtual ~Object() = 0;

  bool isTransparent() const { return isTransparent_; }
  bool isLight() const { return isLight_; }

  virtual std::pair<Object::Intersection, glm::vec3> intersect(const Ray* ray) const;

  virtual glm::vec3 getNormal(const glm::vec3& position) const { return mesh_->getNormal(position); }
  virtual glm::vec3 getRandomSurfacePosition() const { return mesh_->getRandomSurfacePosition(); }
  virtual glm::vec3 getColor(const glm::vec3& position) const { return mesh_->getColor(position); }
  virtual float getArea() const { return mesh_->getArea(); }
  virtual std::string getName() const { return name_; }
  virtual glm::vec3 getIntensity() const { return intensity_; }

  virtual void setIntensity(const glm::vec3& intensity);
  virtual void addIntensity(const glm::vec3& intensity);

protected:
  Mesh* mesh_;

private:
  const bool isTransparent_;
  const bool isLight_;
  const std::string name_;
  glm::vec3 intensity_;

};
#endif
