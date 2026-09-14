
#ifndef RENDERDEBUGSYSTEM_H
#define RENDERDEBUGSYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

class RenderDebugSystem : public System {
public:
  RenderDebugSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  void Update(SDL_Renderer *renderer, const SDL_Rect &camera) {
    assertm(renderer != nullptr, "Renderer must be valid.");
    _drawDebugEntityCollision(renderer, camera);
  }

private:
  void _drawDebugEntityCollision(SDL_Renderer *renderer,
                                 const SDL_Rect &camera) {

    for (const auto &entity : GetEntities()) {
      const auto &transformC{
          EntityManager::Get().GetComponent<TransformComponent>(entity)};
      const auto &boxColliderC{
          EntityManager::Get().GetComponent<BoxColliderComponent>(entity)};

      const SDL_Color color{boxColliderC.GetIsColliding()
                                ? SDL_Color{255, 0, 0}
                                : SDL_Color{0, 255, 0}};

      const SDL_Rect collider{
          static_cast<int>(transformC.position.x + boxColliderC.offset.x -
                           camera.x),
          static_cast<int>(transformC.position.y + boxColliderC.offset.y -
                           camera.y),
          static_cast<int>(boxColliderC.width * transformC.scale.x),
          static_cast<int>(boxColliderC.height * transformC.scale.y),
      };

      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
      SDL_RenderDrawRect(renderer, &collider);
    }
  }
};

#endif
