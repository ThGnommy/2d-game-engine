
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/System.h"
#include <SDL2/SDL.h>
#include <string>

class RenderSystem : public System {
public:
  RenderSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }

  void Update(SDL_Renderer *renderer) {
    for (auto entity : GetEntities()) {
      const auto transform{entity.GetComponent<TransformComponent>()};
      const auto sprite{entity.GetComponent<SpriteComponent>()};

      const SDL_Rect rectObj{static_cast<int>(transform.position.x),
                             static_cast<int>(transform.position.y),
                             sprite.width, sprite.height};

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderFillRect(renderer, &rectObj);
    }
  }
};

#endif
