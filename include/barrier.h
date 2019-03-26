#ifndef BARRIER_H
#define BARRIER_H

#include <vector>

#include "Entity.h"
#include <iostream>

enum barrier_types {
  BARRIER_INDESTRUCTABLE,
  BARRIER_STONE,
  BARRIER_CRATE
};

class Barrier : public Entity {
  public:
    Barrier(World *wrld, float x, float y, int type);
    ~Barrier();

    virtual void Update() override;
    virtual void Draw(BITMAP* buffer) override;
    virtual void Collide(Entity *other) override;

  private:
    void SetType(int type);
    void SetHealth(int health);
    void SetIndestructable(bool indestructable);
    void Destroy();

    int health;
    bool indestructable;
    int type;

    BITMAP* image;
    static BITMAP* images[3];
    static SAMPLE* sample_explode;
};

#endif // BARRIER_H
