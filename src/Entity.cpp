#include "Entity.h"

int Entity::free_id = 0;

Entity::Entity(float x, float y) {
  this -> position = vec2<float>(x, y);
  this -> dimensions = vec2<int>(0, 0);
  this -> id = free_id++;
}

Entity::~Entity() {

}

int Entity::GetID() {
  return id;
}

float Entity::GetX() {
  return position.x;
}

float Entity::GetY() {
  return position.y;
}

vec2<float> Entity::GetPosition() {
  return position;
}

int Entity::GetWidth() {
  return dimensions.x;
}

int Entity::GetHeight() {
  return dimensions.y;
}

vec2<int> Entity::GetDimensions() {
  return dimensions;
}

void Entity::SetDimensions(vec2<int> dimensions) {
  this -> dimensions = dimensions;
}
