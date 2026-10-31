#ifndef PROJECTILELIFECYCLESYSTEM_H
#define PROJECTILELIFECYCLESYSTEM_H

#include "../Components/ProjectileComponent.h"
#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include <SDL2/SDL_timer.h>

class ProjectileLifecycleSystem : public System {
public:
  ProjectileLifecycleSystem() { RequireComponent<ProjectileComponent>(); }

  void Update() {
    for (const auto &entity : GetEntities()) {
      const auto projectile{EntityManager::Get().GetComponent<ProjectileComponent>(entity)};
      if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
        EntityManager::Get().DestroyEntity(entity);
      }
    }
  }
};

#endif
