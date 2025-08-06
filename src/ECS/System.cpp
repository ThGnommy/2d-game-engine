#include "System.h"
#include "Entity.h"
#include <algorithm>

void System::AddEntity(Entity entity) { entities.emplace_back(entity); }

void System::RemoveEntity(Entity entity) {
  entities.erase(
      std::remove_if(entities.begin(), entities.end(),
                     [&entity](const Entity other) { return entity == other; }),
      entities.end());
}

std::vector<Entity> System::GetEntities() const { return entities; }

const Signature &System::GetComponentSignature() const {
  return componentSignature;
}
