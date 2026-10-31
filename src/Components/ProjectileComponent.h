
#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <SDL2/SDL_timer.h>
#include <glm/glm.hpp>

struct ProjectileComponent {
  bool isFriendly{};
  int damage{};
  int duration{};
  int startTime{};

  ProjectileComponent(bool isFriendly = false, int damage = 1, int duration = 3)
      : isFriendly(isFriendly), damage(damage), duration(duration) {
    this->startTime = SDL_GetTicks();
  }
};

#endif
