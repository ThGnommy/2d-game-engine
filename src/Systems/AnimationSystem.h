
#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/System.h"
#include <SDL2/SDL_timer.h>
#include <iostream>

class AnimationSystem : public System {
public:
  AnimationSystem() {
    RequireComponent<SpriteComponent>();
    RequireComponent<AnimationComponent>();
  }

  void Update() {
    for (const auto &entity : GetEntities()) {
      auto &sprite{entity.GetComponent<SpriteComponent>()};
      auto &animation{entity.GetComponent<AnimationComponent>()};

      // TODO:
      // change the current frame
      // change the source rectangle - we have to assume the size (e.g 32px)
      //
      animation.currentFrame = ((SDL_GetTicks() - animation.startTime) *
                                animation.frameSpeedRate / 1000) %
                               animation.numFrames;
      sprite.srcRect.x = animation.currentFrame * sprite.width;
    }
  }
};

#endif
