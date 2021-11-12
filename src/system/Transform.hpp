#ifndef SRC_SYSTEM_TRANSFORM_H_
#define SRC_SYSTEM_TRANSFORM_H_

#include "../util/Vec2.hpp"
#include "Component.hpp"

class C_Transform : public Component {
 public:
  explicit C_Transform(GameObject* owner);

  void setPosition(float x, float y);
  void setPosition(const Vec2<float>& pos);

  void addPosition(float x, float y);
  void addPosition(const Vec2<float>& pos);

  void setX(float x);
  void setY(float y);

  void addX(float x);
  void addY(float y);

  const Vec2<float>& getPosition() const;

 private:
  Vec2<float> position;
};
#endif /* SRC_SYSTEM_TRANSFORM_H_ */