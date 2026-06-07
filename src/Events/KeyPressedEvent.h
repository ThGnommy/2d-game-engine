
#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../ECS/Entity.h"
#include "../EventBus/Event.h"
#include <SDL2/SDL_keycode.h>

class KeyPressedEvent : public Event {
public:
  KeyPressedEvent(SDL_Keycode key) { _key = key; }

  SDL_Keycode GetKeyPressed() { return _key; }

private:
  SDL_Keycode _key{};
};

#endif
