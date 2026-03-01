
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
#include <algorithm>
#include <memory>
#include <vector>

class RenderSystem : public System {
public:
  RenderSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }

  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore) {

    struct RenderableEntity {
      TransformComponent tc{};
      SpriteComponent sc{};
    };

    std::vector<RenderableEntity> sortedEntities{};
    for (const auto &entity : GetEntities()) {
      RenderableEntity e{};
      e.tc = entity.GetComponent<TransformComponent>();
      e.sc = entity.GetComponent<SpriteComponent>();
      sortedEntities.emplace_back(e);
    }

    std::sort(sortedEntities.begin(), sortedEntities.end(),
              [](const RenderableEntity &a, const RenderableEntity &b) {
                return a.sc.zIndex < b.sc.zIndex;
              });

    for (auto entity : sortedEntities) {
      const auto transform{entity.tc};
      const auto sprite{entity.sc};

      SDL_Rect srcRect{sprite.srcRect};

      SDL_Rect dsrRect{static_cast<int>(transform.position.x),
                       static_cast<int>(transform.position.y),
                       static_cast<int>(sprite.width * transform.scale.x),
                       static_cast<int>(sprite.height * transform.scale.y)};

      // in the future we would want to give the possibility to flip the
      // texture from there
      SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId),
                       &srcRect, &dsrRect, transform.rotation, NULL,
                       SDL_FLIP_NONE);
    }
  }
};

#endif
