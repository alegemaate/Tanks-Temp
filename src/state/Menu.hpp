/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
 **/
#ifndef SRC_STATE_MENU_HPP_
#define SRC_STATE_MENU_HPP_

#include <asw/asw.h>
#include <entt/entt.hpp>

#include "../systems/RenderSystem.hpp"
#include "../ui/Button.hpp"
#include "Game.hpp"
#include "State.hpp"

class Menu : public State {
 public:
  explicit Menu(StateEngine& engine) : State(engine) {}

  void init() override;
  void update(const double deltaTime) override;
  void draw() override;
  void cleanup() override {
    // No cleanup
  }

 private:
  Button enemies_up;
  Button enemies_down;
  Button friends_up;
  Button friends_down;
  Button width_up;
  Button width_down;
  Button height_up;
  Button height_down;
  Button bounce_up;
  Button bounce_down;
  Button start;

  asw::Font font;

  entt::registry m_registry;

  entt::dispatcher m_dispatcher;

  RenderSystem m_render_system;
};

#endif  // SRC_STATE_MENU_HPP_
