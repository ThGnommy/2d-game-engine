#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/System.h"


class MovementSystem : public System {
    MovementSystem() {
        // todo: specify the components the system required
        // RequireComponent<...>();
    }

    void Update() {
        // for (auto entity : GetEntities()) {
            // update entity pos based on its velocity
            // every frame of the game loop
        // }
    }
};

#endif
