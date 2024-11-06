#ifndef TRANFORMCOMPONENT_H
#define TRANFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TranformComponent {
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
};

#endif