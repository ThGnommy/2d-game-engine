
#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include <SDL2/SDL_keyboard.h>
#include <string>

class KeyboardControlSystem : public System {
public:
  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent<KeyPressedEvent>(
        this, &KeyboardControlSystem::onKeyPress);
  }

  void Update() {}

protected:
  void onKeyPress(KeyPressedEvent &event) {
    const auto key{event.GetKeyPressed()};
    const auto keySymbol{SDL_GetKeyName(key)};

    Logger::Log(std::string("KeyPressedEvent called with: [ ") +
                std::to_string(static_cast<int>(key)) + " ] -> " + keySymbol);
  }
};

#endif
