#ifndef SYSTEM_COMPONENT_H_
#define SYSTEM_COMPONENT_H_

#include <allegro.h>

class GameObject;

class Component {
 public:
  Component(GameObject* owner) : owner(owner) {}

  virtual void awake(){};

  virtual void start(){};

  virtual void update(float deltaTime){};

  virtual void lateUpdate(float deltaTime){};

  virtual void draw(BITMAP* buffer){};

 protected:
  GameObject* owner;
};

#endif /* SYSTEM_COMPONENT_H_ */