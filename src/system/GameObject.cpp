#include "GameObject.hpp"

GameObject::GameObject() {
  transform = addComponent<C_Transform>();
}

void GameObject::awake() {
  for (unsigned int i = components.size() - 1; i >= 0; i--) {
    components[i]->awake();
  }
}

void GameObject::start() {
  for (unsigned int i = components.size() - 1; i >= 0; i--) {
    components[i]->start();
  }
}

void GameObject::update(float timeDelta) {
  for (unsigned int i = components.size() - 1; i >= 0; i--) {
    components[i]->update(timeDelta);
  }
}

void GameObject::lateUpdate(float timeDelta) {
  for (unsigned int i = components.size() - 1; i >= 0; i--) {
    components[i]->lateUpdate(timeDelta);
  }
}

void GameObject::draw(BITMAP* buffer) {
  for (unsigned int i = components.size() - 1; i >= 0; i--) {
    components[i]->draw(buffer);
  }
}