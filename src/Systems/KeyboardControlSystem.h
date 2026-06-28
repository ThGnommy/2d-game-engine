
#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>

class KeyboardControlSystem : public System {
public:
  KeyboardControlSystem() {
    RequireComponent<KeyboardControlledComponent>();
    RequireComponent<SpriteComponent>();
    RequireComponent<RigidbodyComponent>();
  }

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent<KeyPressedEvent>(
        this, &KeyboardControlSystem::onKeyPressed);
  }

  void Update() {}

  void onKeyPressed(KeyPressedEvent &event) {
    for (auto &entity : GetEntities()) {
      const auto &keyboardControlComp{
          EntityManager::Get().GetComponent<KeyboardControlledComponent>(
              entity)};

      auto &spriteComp{
          EntityManager::Get().GetComponent<SpriteComponent>(entity)};
      auto &rigidBodyComp{
          EntityManager::Get().GetComponent<RigidbodyComponent>(entity)};

      switch (event.GetKeyPressed()) {
      case SDLK_UP:
        rigidBodyComp.velocity = keyboardControlComp.upVelocity;
        spriteComp.srcRect.y = spriteComp.height * 0;
        break;
      case SDLK_RIGHT:
        rigidBodyComp.velocity = keyboardControlComp.rightVelocity;
        spriteComp.srcRect.y = spriteComp.height * 1;
        break;
      case SDLK_DOWN:
        rigidBodyComp.velocity = keyboardControlComp.downVelocity;
        spriteComp.srcRect.y = spriteComp.height * 2;
        break;
      case SDLK_LEFT:
        rigidBodyComp.velocity = keyboardControlComp.leftVelocity;
        spriteComp.srcRect.y = spriteComp.height * 3;
        break;
      }
    }
  };
};

#endif
