#include "Transform.h"

C_Transform::C_Transform(GameObject* owner)
    : Component(owner), position(0.f, 0.f) {}

void C_Transform::setPosition(float x, float y) {
  position.x = x;
  position.y = y;
}

void C_Transform::setPosition(const Coordinate& pos) {
  position = pos;
}

void C_Transform::addPosition(float x, float y) {
  position.x += x;
  position.y += y;
}

void C_Transform::addPosition(Coordinate pos) {
  position.x += pos.x;
  position.y += pos.y;
}

void C_Transform::setX(float x) {
  position.x = x;
}

void C_Transform::setY(float y) {
  position.y = y;
}

void C_Transform::addX(float x) {
  position.x += x;
}

void C_Transform::addY(float y) {
  position.y += y;
}

const Coordinate& C_Transform::getPosition() const {
  return position;
}