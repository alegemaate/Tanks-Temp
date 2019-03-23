/**
 * Entity.c
 * General purpose entity class
 * Allan Legemaate
 * 23-03-2019
 **/

#ifndef ENTITY_H
#define ENTITY_H

#include "vec2.h"

class Entity {
  public:
    Entity(float x, float y);
    virtual ~Entity();

    int GetID();
    float GetX();
    float GetY();
    vec2<float> GetPosition();

    int GetWidth();
    int GetHeight();
    vec2<int> GetDimensions();
    void SetDimensions(vec2<int> dimensions);

  private:
    // Unique entity id
    int id;

    // Position
    vec2<float> position;

    // Size
    vec2<int> dimensions;

    // ID iterator
    static int free_id;
};

#endif // ENTITY_H
