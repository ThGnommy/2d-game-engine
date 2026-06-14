
#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "../Events/CollisionEvent.h"
#include "../EventBus/EventBus.h"
#include <SDL2/SDL.h>
#include <cstddef>
#include <string>

class CollisionSystem : public System {
public:
  CollisionSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  void Update(std::unique_ptr<EventBus>& eventBus) {
    const auto &entities{GetEntities()};

    // Compare each pair of entities only once (j starts from i+1
    // to avoid duplicate checks and self-collisions)
    for (size_t i{}; i < entities.size(); i++) {
      const auto entityA{_getColliderInfo(entities[i])};

      for (size_t j{i + 1}; j < entities.size(); j++) {
        const auto entityB{_getColliderInfo(entities[j])};

        // avoid check the same entity
        if (entities[i] == entities[j]) {
          continue;
        }

        // set the collision flag on both entities
        const auto colliding{_aabb(entityA, entityB)};
        EntityManager::Get().GetComponent<BoxColliderComponent>(entities[i]).SetIsColliding(
            colliding);
        EntityManager::Get().GetComponent<BoxColliderComponent>(entities[j]).SetIsColliding(
            colliding);

        if (colliding) {
          Logger::Warn(std::to_string(entities[i].GetId()) +
                       " is colliding with " +
                       std::to_string(entities[j].GetId()));
          eventBus->EmitEvent<CollisionEvent>(entities[i], entities[j]);
        }
      }
    }
  }

private:
  struct ColliderInfo {
    double x{};
    double y{};
    double collisionWidth{};
    double collisionHeight{};
  };

  ColliderInfo _getColliderInfo(const Entity &entity) {
    const auto &transformC{EntityManager::Get().GetComponent<TransformComponent>(entity)};
    const auto &boxColliderC{EntityManager::Get().GetComponent<BoxColliderComponent>(entity)};

    return {
        transformC.position.x + boxColliderC.offset.x,
        transformC.position.y + boxColliderC.offset.y,
        boxColliderC.width * transformC.scale.x,
        boxColliderC.height * transformC.scale.y,
    };
  }

  // AABB (Axis-Aligned Bounding Box) collision detection:
  // two rectangles overlap if and only if
  // there is no separation on either axis (X and Y)
  bool _aabb(const ColliderInfo &entityA, const ColliderInfo &entityB) {
    if (entityA.x < entityB.x + entityB.collisionWidth &&
        entityA.x + entityA.collisionWidth > entityB.x &&
        entityA.y < entityB.y + entityB.collisionHeight &&
        entityA.y + entityA.collisionHeight > entityB.y) {
      return true;
    }

    return false;
  }
};

#endif
