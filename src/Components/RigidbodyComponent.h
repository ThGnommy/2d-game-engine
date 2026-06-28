#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
struct RigidbodyComponent {
  RigidbodyComponent(glm::vec2 velocity = glm::vec2(0, 0))
      : velocity(velocity) {}

  glm::vec2 velocity{};
};

#endif
