#include "ECS.h"
#include "../Logger/Logger.h"
#include "algorithm"
#include <cassert>

int IComponent::nextId = 0;

int Entity::GetId() const { return id; }

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

Entity EntityManager::CreateEntity() {

  unsigned int entityId{numEntities++};

  Entity entity(entityId);

  entitiesToBeCreated.insert(entity);

  // Make sure the entityComponentSignatures vector can accomodate the new
  // entity
  if (entityId >= entityComponentSignatures.size()) {
    entityComponentSignatures.resize(entityId + 1);
  }

  Logger::Log("Entity created with id: " + std::to_string(entityId));

  return entity;
}

void EntityManager::Update() {
  // Add the entities that are waiting to be created to the active Systems

  if (!entitiesToBeCreated.empty()) {
    for (const auto entity : entitiesToBeCreated) {
      AddEntityToSystem(entity);
    }

    entitiesToBeCreated.clear();
  }
}

void EntityManager::DeleteEntity(Entity *entity) {}

void EntityManager::AddEntityToSystem(Entity entity) {
  const auto entityId{entity.GetId()};
  const auto &entityComponentSignature{entityComponentSignatures[entityId]};

  for (const auto &system : systems) {
    const auto &systemCompSig{system.second->GetComponentSignature()};
    bool isInterested{(entityComponentSignature & systemCompSig) ==
                      systemCompSig};

    if (isInterested) {
      system.second->AddEntity(entity);
    }
  }
}
