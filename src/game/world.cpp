#include "world.h"

#include <algorithm>

// Adds particle to global particle handler
void World::addParticle(Particle* particle) {
  particles.push_back(particle);
}

// Updates world
void World::update() {
  // Update particles
  for (auto it = particles.begin(); it != particles.end();) {
    if ((*it)->getDead()) {
      it = particles.erase(it);
    } else {
      (*it)->logic();
      it++;
    }
  }
}

// Draw world
void World::draw(BITMAP* buffer) {
  // Draw particles
  for (auto it = particles.begin(); it != particles.end(); it++) {
    (*it)->draw(buffer);
  }
}
