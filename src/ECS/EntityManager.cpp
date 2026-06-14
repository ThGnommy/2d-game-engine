#include "EntityManager.h"
#include "Component.h"

int IComponent::nextId = 0;

Entity EntityManager::CreateEntity() {
  unsigned int entityId{};

  if (_freeIds.empty()) {
    entityId = _numEntities++;
    // Make sure the entityComponentSignatures vector can accomodate the new
    // entity
    if (entityId >= _entityComponentSignatures.size()) {
      _entityComponentSignatures.resize(entityId + 1);
    }
  } else {
    entityId = _freeIds.front();
    _freeIds.pop_front();
  }

  Entity entity(entityId);

  _entitiesToBeCreated.insert(entity);

  Logger::Log("Entity created with id: " + std::to_string(entityId));

  return entity;
}

void EntityManager::DestroyEntity(Entity entity) {
  _entitiesToBeKilled.insert(entity);
  Logger::Log("Entity: " + std::to_string(entity.GetId()) + " destroyed!");
}

void EntityManager::Update() {
  // Add the entities that are waiting to be created to the active Systems

  for (const auto entity : _entitiesToBeCreated) {
    _addEntityToSystem(entity);
  }

  _entitiesToBeCreated.clear();

  for (auto &entity : _entitiesToBeKilled) {
    _removeEntityFromSystems(entity);
    _entityComponentSignatures[entity.GetId()].reset();

    // Make the entity id available to be reused
    _freeIds.push_back(entity.GetId());
  }

  _entitiesToBeKilled.clear();
}

void EntityManager::_addEntityToSystem(Entity entity) {
  const auto entityId{entity.GetId()};
  const auto &entityComponentSignature{_entityComponentSignatures[entityId]};

  for (const auto &system : _systems) {
    const auto &systemCompSig{system.second->GetComponentSignature()};
    bool isInterested{(entityComponentSignature & systemCompSig) ==
                      systemCompSig};

    if (isInterested) {
      system.second->AddEntity(entity);
    }
  }
}

void EntityManager::_removeEntityFromSystems(Entity entity) {
  for (const auto &system : _systems) {
    system.second->RemoveEntity(entity);
  }
}
