#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
struct RigidbodyComponent {
    glm::vec2 velocity{};

    RigidbodyComponent(glm::vec2 velocity = glm::vec2(0, 0)) : velocity(velocity) {}
};

#endif
