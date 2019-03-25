/**
 * Entity.c
 * General purpose entity class
 * Allan Legemaate
 * 23-03-2019
 **/

#ifndef ENTITY_H
#define ENTITY_H

#include "vec2.h"
#include <allegro.h>

class world;

class Entity {
  public:
    Entity(world *wrld, float x, float y);
    virtual ~Entity();

    int GetID();
    float GetX();
    float GetY();
    vec2<float> GetPosition();

    int GetWidth();
    int GetHeight();
    vec2<int> GetDimensions();
    void SetDimensions(vec2<int> dimensions);

    vec2<float> GetVelocity();

    void SetVelocity(vec2<float> velocity);

    virtual void Update() = 0;
    virtual void Draw(BITMAP* buffer) = 0;

    virtual void Collide(Entity* other);

  protected:
    // Position
    vec2<float> position;

    // Size
    vec2<int> dimensions;

    // Velocity
    vec2<float> velocity;

    // Pointer to game world
    world *wrld;

  private:
    // Unique entity id
    int id;

    // ID iterator
    static int free_id;

};

#endif // ENTITY_H
