#ifndef SRC_SYSTEM_TRANSFORM_H_
#define SRC_SYSTEM_TRANSFORM_H_

#include "../util/coordinate.h"
#include "Component.h"

class C_Transform : public Component {
 public:
  C_Transform(GameObject* owner);

  void setPosition(float x, float y);
  void setPosition(const Coordinate& pos);

  void addPosition(float x, float y);
  void addPosition(Coordinate pos);

  void setX(float x);
  void setY(float y);

  void addX(float x);
  void addY(float y);

  const Coordinate& GetPosition() const;

 private:
  Coordinate position;
};
#endif /* SRC_SYSTEM_TRANSFORM_H_ */