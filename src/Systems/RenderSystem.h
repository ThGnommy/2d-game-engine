
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/System.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <memory>

class RenderSystem : public System {
public:
  RenderSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }

  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore) {

    // const auto sortedEntities{std::sort(GetEntities().begin(), GetEntities().end)};

    for (auto entity : GetEntities()) {
      const auto transform{entity.GetComponent<TransformComponent>()};
      const auto sprite{entity.GetComponent<SpriteComponent>()};

      SDL_Rect srcRect{sprite.srcRect};

      SDL_Rect dsrRect{static_cast<int>(transform.position.x),
                       static_cast<int>(transform.position.y),
                       static_cast<int>(sprite.width * transform.scale.x),
                       static_cast<int>(sprite.height * transform.scale.y)};

      // in the future we would want to give the possibility to flip the texture
      // from there
      SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId),
                       &srcRect, &dsrRect, transform.rotation, NULL,
                       SDL_FLIP_NONE);
    }
  }
};

#endif
