
#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/System.h"
#include <cstddef>

class CollisionSystem : public System {
public:
  CollisionSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  void Update() {
    const auto &entities{GetEntities()};

    // Compare each pair of entities only once (j starts from i+1
    // to avoid duplicate checks and self-collisions)
    for (size_t i{}; i < entities.size(); i++) {
      const auto entityA{_getColliderInfo(entities[i])};

      for (size_t j{i + 1}; j < entities.size(); j++) {
        const auto entityB{_getColliderInfo(entities[j])};

        // set the collision flag on both entities
        const auto colliding{_aabb(entityA, entityB)};
        entities[i].GetComponent<BoxColliderComponent>().SetIsColliding(
            colliding);
        entities[j].GetComponent<BoxColliderComponent>().SetIsColliding(
            colliding);
      }
    }
  }

private:
  struct ColliderInfo {
    float x{};
    float y{};
    int collisionWidth{};
    int collisionHeight{};
  };

  ColliderInfo _getColliderInfo(const Entity &entity) {
    const auto &transformC{entity.GetComponent<TransformComponent>()};
    const auto &boxColliderC{entity.GetComponent<BoxColliderComponent>()};

    return {
        transformC.position.x,
        transformC.position.y,
        boxColliderC.width,
        boxColliderC.height,
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
