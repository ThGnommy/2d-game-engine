
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"
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

  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore,
              const SDL_Rect &camera) {

    struct RenderableEntity {
      TransformComponent tc{};
      SpriteComponent sc{};
    };

    std::vector<RenderableEntity> sortedEntities{};
    for (const auto &entity : GetEntities()) {
      RenderableEntity e{};
      e.tc = EntityManager::Get().GetComponent<TransformComponent>(entity);
      e.sc = EntityManager::Get().GetComponent<SpriteComponent>(entity);
      sortedEntities.emplace_back(e);
    }

    // sort entities by z-index
    std::sort(sortedEntities.begin(), sortedEntities.end(),
              [](const RenderableEntity &a, const RenderableEntity &b) {
                return a.sc.zIndex < b.sc.zIndex;
              });

    // here we actually render all the sprites
    for (auto entity : sortedEntities) {
      const auto transform{entity.tc};
      const auto sprite{entity.sc};

      SDL_Rect srcRect{sprite.srcRect};

      SDL_Rect dsrRect{static_cast<int>(transform.position.x -
                                        (sprite.isFixed ? 0 : camera.x)),
                       static_cast<int>(transform.position.y -
                                        (sprite.isFixed ? 0 : camera.y)),
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
