
#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL2/SDL_rect.h>
#include <algorithm>
#include <string>

class CameraMovementSystem : public System {
public:
  CameraMovementSystem() {
    RequireComponent<CameraFollowComponent>();
    RequireComponent<TransformComponent>();
  }

  void Update(SDL_Rect &camera) {
    for (auto &entity : GetEntities()) {
      auto &transform{
          EntityManager::Get().GetComponent<TransformComponent>(entity)};

      const int maxX{std::max(0, Game::MapWidth - camera.w)};
      const int maxY{std::max(0, Game::MapHeight - camera.h)};

      camera.x = std::clamp(
          static_cast<int>(transform.position.x - camera.w / 2), 0, maxX);
      camera.y = std::clamp(
          static_cast<int>(transform.position.y - camera.h / 2), 0, maxY);
    }
  }
};

#endif
