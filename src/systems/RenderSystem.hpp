

#ifndef SRC_SYSTEMS_RENDER_HPP_
#define SRC_SYSTEMS_RENDER_HPP_

#include <allegro.h>
#include <entt/fwd.hpp>

class RenderSystem final {
 public:
  RenderSystem() = default;

  ~RenderSystem() = default;

  void render(BITMAP* buffer, entt::registry& registry);
};

#endif  // SRC_SYSTEMS_RENDER_HPP_
