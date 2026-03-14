#ifndef ANIMAIONCOMPONENT_H
#define ANIMAIONCOMPONENT_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_timer.h>
#include <string>

struct AnimationComponent {
  int numFrames{};
  int currentFrame{};
  int frameSpeedRate{};
  bool isLoop{};
  int startTime{};

  AnimationComponent(int numFrames = 1, int frameSpeedRate = 1,
                     bool isLoop = true) {

    this->numFrames = numFrames;
    this->currentFrame = 1;
    this->frameSpeedRate = frameSpeedRate;
    this->isLoop = isLoop;
    this->startTime = SDL_GetTicks();
  }
};

#endif
