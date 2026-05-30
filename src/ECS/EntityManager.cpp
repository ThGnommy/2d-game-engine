#include "EntityManager.h"
#include "Component.h"
#include "Entity.h"

int IComponent::nextId = 0;

Entity EntityManager::CreateEntity() {
  unsigned int entityId{};

  if (freeIds.empty()) {
    entityId = numEntities++;
    // Make sure the entityComponentSignatures vector can accomodate the new
    // entity
    if (entityId >= entityComponentSignatures.size()) {
      entityComponentSignatures.resize(entityId + 1);
    }
  } else {
    entityId = freeIds.front();
    freeIds.pop_front();
  }

  Entity entity(entityId);

  entitiesToBeCreated.insert(entity);

  Logger::Log("Entity created with id: " + std::to_string(entityId));

  return entity;
}

void EntityManager::DestroyEntity(Entity entity) {
  entitiesToBeKilled.insert(entity);
  Logger::Log("Entity: " + std::to_string(entity.GetId()) + " destroyed!");
}

void EntityManager::Update() {
  // Add the entities that are waiting to be created to the active Systems

  for (const auto entity : entitiesToBeCreated) {
    AddEntityToSystem(entity);
  }

  entitiesToBeCreated.clear();

  for (auto &entity : entitiesToBeKilled) {
    RemoveEntityFromSystems(entity);
    entityComponentSignatures[entity.GetId()].reset();

    // Make the entity id available to be reused
    freeIds.push_back(entity.GetId());
  }

  entitiesToBeKilled.clear();
}

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

void EntityManager::RemoveEntityFromSystems(Entity entity) {
  for (const auto &system : systems) {
    system.second->RemoveEntity(entity);
  }
}
