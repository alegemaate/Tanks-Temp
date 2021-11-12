#ifndef SYSTEM_GAMEOBJECT_H_
#define SYSTEM_GAMEOBJECT_H_

#include <allegro.h>
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Transform.hpp"

class GameObject {
 public:
  GameObject();

  void awake();

  void start();

  void update(float deltaTime);

  void lateUpdate(float deltaTime);

  void draw(BITMAP* buffer);

  template <typename T>
  std::shared_ptr<T> addComponent() {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must derive from Component");
    for (auto& existingComponent : components) {
      if (std::dynamic_pointer_cast<T>(existingComponent)) {
        return std::dynamic_pointer_cast<T>(existingComponent);
      }
    }

    std::shared_ptr<T> newComponent = std::make_shared<T>(this);
    components.push_back(newComponent);
    return newComponent;
  }

  template <typename T>
  std::shared_ptr<T> getComponent() {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must derive from Component");
    for (auto& existingComponent : components) {
      if (std::dynamic_pointer_cast<T>(existingComponent)) {
        return std::dynamic_pointer_cast<T>(existingComponent);
      }
    }

    return nullptr;
  }

 private:
  std::vector<std::shared_ptr<Component>> components;

  std::shared_ptr<C_Transform> transform;
};

#endif /* SYSTEM_GAMEOBJECT_H_ */