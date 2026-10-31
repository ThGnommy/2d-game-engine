#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <SDL2/SDL_rect.h>
#include <glm/vec2.hpp>

struct HealthComponent {
  HealthComponent(unsigned int health = 10) : health(health) {}

  unsigned int health{};
};

#endif
