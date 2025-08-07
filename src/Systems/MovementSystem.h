#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/System.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../ECS/Entity.h"
#include <string>


class MovementSystem : public System {
  public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidbodyComponent>();
    }

    void Update(double deltaTime) {
        for (auto entity : GetEntities()) {
            // update entity pos based on its velocity
            // every frame of the game loop
            auto& transform{entity.GetComponent<TransformComponent>()};
            const auto rigidbody{entity.GetComponent<RigidbodyComponent>()};

            transform.position.x += rigidbody.velocity.x * deltaTime;
            transform.position.y += rigidbody.velocity.y * deltaTime;

            Logger::Log("Entity Movement: " + std::to_string(transform.position.x) + " : " + std::to_string(transform.position.y));
        }
    }
};

#endif
