#include "ECS.h"
#include "../Logger/Logger.h"
#include "algorithm"

int IComponent::nextId = 0;

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

template <typename T, typename ...TArgs>
void EntityManager::AddComponent(const Entity entity, TArgs&& ...args) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};

    // Increment the capacity for accomodate the new component id
    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }

    assertm(!componentPools[componentId], "That index should be empty at this time");

    if (!componentPools[componentId]) {
        Pool<T>* newComponentPool = new Pool<T>();
        componentPools[componentId] = newComponentPool;
    }

    // Gets the pool of component values for the component type
    Pool<T>* componentPool{Pool<T>(componentPool[componentId])};

    if (entityId >= componentPool->GetSize()) {
        componentPool->resize(numEntities);
    }

    // Create a new component object of type T, 
    // and forward the various paramenters to the constructor
    T newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);
    // change the component signature of the entity and set the component id on 
    // the bitset to 1
    entityComponentSignatures[entityId].set(componentId);
}

template <typename T> 
void EntityManager::RemoveComponent(const Entity entity) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};
    entityComponentSignatures[entityId].set(componentId, false);
}

template <typename T>
bool EntityManager::HasComponent(Entity entity) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};

    return entityComponentSignatures[entityId].test(componentId);
}


template <typename TSystem, typename ...TArgs> 
void EntityManager::AddSystem(TArgs&& ...args) {

    TSystem* newSystem{new TSystem(std::forward<TArgs>(args)...)};
    std::type_index index{std::type_index(typeid(TSystem))};

    systems.insert(std::make_pair(index, newSystem));
}


template <typename TSystem> 
void EntityManager::RemoveSystem() {
    // systems.erase()
}
