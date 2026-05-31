#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/Entity.h"
#include "../EventBus/Event.h"

class CollisionEvent : public Event {
public:
  CollisionEvent(Entity a, Entity b) : a(a), b(b) {}

  Entity a;
  Entity b;
};

#endif
