#ifndef POWERUP_H
#define POWERUP_H

#include "Entity.h"

enum powerup_types {
  POWERUP_HEALTH,
  POWERUP_TANK_SPEED,
  POWERUP_BULLET_SPEED,
  POWERUP_BULLET_RATE
};

class Powerup : public Entity {
  public:
    Powerup(World *wrld, int x, int y, int type);
    virtual ~Powerup();

    virtual void Update() override;
    virtual void Draw(BITMAP *buffer) override;

    int GetType();
  protected:

  private:
    void SetType(int type);
    int type;

    BITMAP *image;
    static BITMAP* images[4];
};

#endif // POWERUP_H
