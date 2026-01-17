#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec2 position{};
    glm::vec2 scale{};
    double rotation{};

    TransformComponent(glm::vec2 position = {0, 0}, glm::vec2 scale = {1.0, 1.0}, double rotation = 0.0)
        : position(position), scale(scale), rotation(rotation) {}
};

#endif
