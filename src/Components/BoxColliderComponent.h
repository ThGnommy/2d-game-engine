
#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include "glm/glm.hpp"

struct BoxColliderComponent {
  BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = {})
      : width(width), height(height), offset(offset) {}

  int width{};
  int height{};
  glm::vec2 offset{};

  bool GetIsColliding() { return _isColliding; }
  void SetIsColliding(bool value) {
    _isColliding = value;
  }

private:
  bool _isColliding{};
};

#endif
