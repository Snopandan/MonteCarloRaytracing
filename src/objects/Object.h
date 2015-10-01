#ifndef OBJECT_H
#define OBJECT_H

class Object {
public:
  Object(bool isTransparent);
  virtual ~Object() = 0;

  bool isTransparent() const { return isTransparent_; }

private:
  const bool isTransparent_;
};
#endif
