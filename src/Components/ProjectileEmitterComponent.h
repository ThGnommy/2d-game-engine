#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include <SDL2/SDL_timer.h>
#include <glm/glm.hpp>

struct ProjectileEmitterComponent {
  glm::vec2 velocity{};
  int repeatFrequency{};
  int lifetime{};
  int damage{};
  int isFriendly{};
  unsigned int lastEmissionTime{};

  ProjectileEmitterComponent(glm::vec2 velocity = {0, 0},
                             int repeatFrequency = 1000, int lifetime = 1000,
                             int damage = 1, bool isFriendly = false,
                             unsigned int lastEmissionTime = SDL_GetTicks())
      : velocity(velocity), repeatFrequency(repeatFrequency),
        lifetime(lifetime), damage(damage), isFriendly(isFriendly),
        lastEmissionTime(lastEmissionTime) {}
};

#endif
