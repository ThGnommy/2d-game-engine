#include "ECS.h"
#include "../Logger/Logger.h"

int Entity::GetId() const {
    return id;
}

void System::AddEntity(Entity entity) {
   entities.emplace_back(entity);     
}

void System::RemoveEntity(Entity entity) {
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](const Entity other) {
       return entity == other; 
    }), entities.end());
}

std::vector<Entity> System::GetEntities() const {
    return entities;
}

const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

Entity EntityManager::CreateEntity() {

    unsigned int entityId{numEntities++};

    Entity entity(entityId);

    entitiesToBeCreated.insert(entity);

    Logger::Log("Entity created with id: " + std::to_string(entityId));

    return entity;
}

void EntityManager::DeleteEntity(Entity* entity) {

}

void EntityManager::Update() {

}