#ifndef KEYBOARDCONTROLLEDCOMPONENT_H
#define KEYBOARDCONTROLLEDCOMPONENT_H

#include <SDL2/SDL_rect.h>
#include <glm/vec2.hpp>

struct KeyboardControlledComponent {
  KeyboardControlledComponent(double upVelocity = 0., double rightVelocity = 0.,
                              double downVelocity = 0.,
                              double leftVelocity = 0.) {
    this->upVelocity = glm::vec2{0., -upVelocity};
    this->rightVelocity = glm::vec2{rightVelocity, 0.};
    this->downVelocity = glm::vec2{0., downVelocity};
    this->leftVelocity = glm::vec2{-rightVelocity, 0.};
  }

  glm::vec2 upVelocity{};
  glm::vec2 rightVelocity{};
  glm::vec2 downVelocity{};
  glm::vec2 leftVelocity{};
};

#endif
