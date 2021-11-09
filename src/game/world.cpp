#include "world.h"

#include <algorithm>

// Adds particle to global particle handler
void World::addParticle(Particle* particle) {
  particles.push_back(particle);
}

// Updates world
void World::update() {
  for (auto* const& particle : particles) {
    particle->logic();
  }

  particles.erase(
      std::remove_if(particles.begin(), particles.end(),
                     [](auto* const& particle) { return particle->getDead(); }),
      particles.end());
}

// Draw world
void World::draw(BITMAP* buffer) {
  // Draw particles
  for (auto* const& particle : particles) {
    particle->draw(buffer);
  }
}
