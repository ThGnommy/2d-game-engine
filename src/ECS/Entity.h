#pragma once

#include "Component.h"
#include <bitset>
#include <cassert>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "../Logger/Logger.h"
#include "EntityManager.h"

class Entity {
public:
  bool operator==(const Entity &other) const { return id == other.GetId(); }

  bool operator!=(const Entity &other) const { return id != other.GetId(); }

  bool operator>(const Entity &other) const { return id > other.GetId(); }

  bool operator<(const Entity &other) const { return id < other.GetId(); }

  Entity &operator=(const Entity &other) = default;

  Entity(int id) : id(id) {};

  int GetId() const;

  template <typename T, typename... TArgs>
  void AddComponent(TArgs &&...args) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{GetId()};

    // Increment the capacity for accomodate the new component id
    if (componentId >= _getEntityManager().componentPools.size()) {
      _getEntityManager().componentPools.resize(componentId + 1, nullptr);
    }

    assertm(!_getEntityManager().componentPools[componentId],
            "That index should be empty at this time");

    if (!_getEntityManager().componentPools[componentId]) {
      std::shared_ptr<Pool<T>> newComponentPool = std::make_shared<Pool<T>>();
      _getEntityManager().componentPools[componentId] = newComponentPool;
    }

    // Gets the pool of component values for the component type
    std::shared_ptr<Pool<T>> componentPool{
        std::static_pointer_cast<Pool<T>>(_getEntityManager().componentPools[componentId])};

    if (entityId >= componentPool->GetSize()) {
      componentPool->Resize(_getEntityManager().numEntities);
    }

    // Create a new component object of type T,
    // and forward the various paramenters to the constructor
    T newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);
    // change the component signature of the entity and set the component id on
    // the bitset to 1
    _getEntityManager().entityComponentSignatures[entityId].set(componentId);

    Logger::Log("Component id = " + std::to_string(componentId) + " was added to entity id " + std::to_string(entityId));
  }

  template <typename T> void RemoveComponent(const Entity entity) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};
    _getEntityManager().entityComponentSignatures[entityId].set(componentId, false);
  }

  template <typename T> bool HasComponent(Entity entity) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};

    return _getEntityManager().entityComponentSignatures[entityId].test(componentId);
  }

private:
  EntityManager& _getEntityManager() const;
  int id{};
};
