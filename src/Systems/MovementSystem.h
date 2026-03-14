#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../Components/RigidbodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/System.h"

class MovementSystem : public System {
public:
  MovementSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<RigidbodyComponent>();
  }

  void Update(double deltaTime) {
    for (const auto &entity : GetEntities()) {
      // update entity pos based on its velocity
      // every frame of the game loop
      auto &transform{entity.GetComponent<TransformComponent>()};
      const auto rigidbody{entity.GetComponent<RigidbodyComponent>()};

      transform.position.x += rigidbody.velocity.x * deltaTime;
      transform.position.y += rigidbody.velocity.y * deltaTime;
    }
  }
};

#endif
