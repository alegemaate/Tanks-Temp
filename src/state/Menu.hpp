/**
 * Menu
 * Allan Legemaate
 * 20/08/2017
 **/
#ifndef SRC_STATE_MENU_HPP_
#define SRC_STATE_MENU_HPP_

#include <allegro.h>
#include <entt/entt.hpp>

#include "../systems/RenderSystem.hpp"
#include "../ui/Button.hpp"
#include "Game.hpp"
#include "State.h"

class Menu : public State {
 public:
  Menu();

  void update(const double deltaTime) override;
  void draw() override;

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

  BITMAP* buffer;

  entt::registry m_registry;

  entt::dispatcher m_dispatcher;

  RenderSystem m_render_system;
};

#endif  // SRC_STATE_MENU_HPP_
