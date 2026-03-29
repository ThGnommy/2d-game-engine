
#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/System.h"

class CollisionSystem : public System {
public:
  CollisionSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  void Update() {
    for (int i{}; i < GetEntities().size(); i++) {
      const auto &transformComponent{
          GetEntities()[i].GetComponent<TransformComponent>()};
      const auto &boxColliderComponent{
          GetEntities()[i].GetComponent<BoxColliderComponent>()};
      EntityInfo firstEntity{.x = transformComponent.position.x,
                             .y = transformComponent.position.y,
                             .collisionWidth = boxColliderComponent.width,
                             .collisionHeight = boxColliderComponent.height};

      for (int j{i + 1}; j < GetEntities().size(); j++) {
        const auto &transformComponent{
            GetEntities()[j].GetComponent<TransformComponent>()};
        const auto &boxColliderComponent{
            GetEntities()[j].GetComponent<BoxColliderComponent>()};
        EntityInfo secondEntity{.x = transformComponent.position.x,
                                .y = transformComponent.position.y,
                                .collisionWidth = boxColliderComponent.width,
                                .collisionHeight = boxColliderComponent.height};

        // set the collision on both entities
        const auto colliding{_aabb(firstEntity, secondEntity)};
        GetEntities()[i].GetComponent<BoxColliderComponent>().SetIsColliding(
            colliding);
        GetEntities()[j].GetComponent<BoxColliderComponent>().SetIsColliding(
            colliding);
      }
    }
  }

private:
  struct EntityInfo {
    float x{};
    float y{};
    int collisionWidth{};
    int collisionHeight{};
  };

  bool _aabb(const EntityInfo &entity1, const EntityInfo &entity2) {
    if (entity1.x < entity2.x + entity2.collisionWidth &&
        entity1.x + entity1.collisionWidth > entity2.x &&
        entity1.y < entity2.y + entity2.collisionHeight &&
        entity1.y + entity1.collisionHeight > entity2.y) {
      return true;
    }

    return false;
  }
};

#endif
