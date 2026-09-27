#ifndef PROJECTILEEMITTERSYSTEM_H
#define PROJECTILEEMITTERSYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "glm/ext/vector_float2.hpp"
#include <SDL2/SDL_timer.h>
#include <string>

class ProjectileEmitterSystem : public System {
public:
  ProjectileEmitterSystem() {
    RequireComponent<ProjectileEmitterComponent>();
    RequireComponent<TransformComponent>();
  }

  void Update() {
    for (const auto &entity : GetEntities()) {
      auto &projectileEmitter{EntityManager::Get().GetComponent<ProjectileEmitterComponent>(entity)};
      const auto transform{EntityManager::Get().GetComponent<TransformComponent>(entity)};

      bool canEmit{SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency};

      if (canEmit) {
        glm::vec2 projectilePosition{transform.position};
        // if we have a parent sprite, we want to emit them from middle of the sprite
        if (EntityManager::Get().HasComponent<SpriteComponent>(entity)) {
          const auto &sprite{EntityManager::Get().GetComponent<SpriteComponent>(entity)};

          projectilePosition.x += sprite.width * 0.5 * transform.scale.x;
          projectilePosition.y += sprite.height * 0.5 * transform.scale.y;
        }
        _addProjectileEntity(projectilePosition, projectileEmitter.velocity);
        projectileEmitter.lastEmissionTime = SDL_GetTicks();
      }
    }
  }

private:
  void _addProjectileEntity(const glm::vec2 startPos, const glm::vec2 velocity) {

    auto entity{EntityManager::Get().CreateEntity()};
    EntityManager::Get().AddComponent<TransformComponent>(entity, startPos);
    EntityManager::Get().AddComponent<RigidbodyComponent>(entity, velocity);
    EntityManager::Get().AddComponent<SpriteComponent>(entity, "bullet", 4, 4, 4);
    EntityManager::Get().AddComponent<BoxColliderComponent>(entity, 4, 4);
  }
};

#endif
