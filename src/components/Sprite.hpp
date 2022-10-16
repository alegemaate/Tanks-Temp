
#ifndef SRC_COMPONENTS_SPRITE_HPP_
#define SRC_COMPONENTS_SPRITE_HPP_

#include <string>

class Sprite final {
 public:
  Sprite(const std::string key) noexcept;

  Sprite(Sprite&&) noexcept;

  ~Sprite() noexcept = default;

  Sprite& operator=(Sprite&&) noexcept;

 public:
  std::string key;
};

#endif  // SRC_COMPONENTS_SPRITE_HPP_