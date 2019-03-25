#ifndef BARRIER_H
#define BARRIER_H

#include <vector>

#include "Entity.h"

class world;
class Entity;

enum barrier_types {
  BARRIER_INDESTRUCTABLE,
  BARRIER_STONE,
  BARRIER_CRATE
};

class Barrier : public Entity {
  public:
    Barrier(world *wrld, float x, float y, int type);
    ~Barrier();

    virtual void Update() override;
    virtual void Draw(BITMAP* buffer) override;
    virtual void Collide(Entity *other) override;

  private:
    void SetType(int type);
    void SetHealth(int health);
    void SetIndestructable(bool indestructable);
    void Destroy(int x, int y, int velocity, int amount, int life);

    int health;
    bool indestructable;
    int type;

    BITMAP* image;
    static BITMAP* images[3];
    static SAMPLE* sample_explode;
};

#endif // BARRIER_H
