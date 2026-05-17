
#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include "glm/glm.hpp"

struct BoxColliderComponent {
  BoxColliderComponent(double width = 0., double height = 0., glm::vec2 offset = {})
      : width(width), height(height), offset(offset) {}

  double width{};
  double height{};
  glm::vec2 offset{};

  bool GetIsColliding() const { return _isColliding; }
  void SetIsColliding(bool value) {
    _isColliding = value;
  }

private:
  bool _isColliding{};
};

#endif
