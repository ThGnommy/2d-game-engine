#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "../Logger/Logger.h"
#include "Entity.h"
#include "IPool.h"
#include "System.h"
#include "Types.h"
#include <bitset>
#include <cassert>
#include <deque>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

/*
@brief Entity manager.
*/
class EntityManager {
public:
  static EntityManager &Get() {
    static EntityManager instance;
    return instance;
  }

  void Update();
  Entity CreateEntity();

  void DestroyEntity(Entity entity);

  template <typename TSystem, typename... TArgs>
  void AddSystem(TArgs &&...args) {

    std::shared_ptr<TSystem> newSystem{
        std::make_shared<TSystem>(std::forward<TArgs>(args)...)};
    std::type_index index{std::type_index(typeid(TSystem))};

    _systems.insert(std::make_pair(index, newSystem));
  }

  template <typename TSystem> void RemoveSystem() {
    const auto system{_systems.find(std::type_index(typeid(TSystem)))};
    _systems.erase(system);
  }

  template <typename TSystem> bool HasSystem() const {
    const auto index{std::type_index(typeid(TSystem))};
    return _systems.find(index) != _systems.end();
  }

  template <typename TSystem> TSystem &GetSystem() const {
    auto system{_systems.find(std::type_index(typeid(TSystem)))};
    return *(std::static_pointer_cast<TSystem>(system->second));
  }

  template <typename T, typename... TArgs>
  void AddComponent(Entity entity, TArgs &&...args) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};

    // Increment the capacity for accomodate the new component id
    if (static_cast<size_t>(componentId) >= _componentPools.size()) {
      _componentPools.resize(componentId + 1, nullptr);
    }

    if (!_componentPools[componentId]) {
      std::shared_ptr<Pool<T>> newComponentPool = std::make_shared<Pool<T>>();
      _componentPools[componentId] = newComponentPool;
    }

    // Gets the pool of component values for the component type
    std::shared_ptr<Pool<T>> componentPool{
        std::static_pointer_cast<Pool<T>>(_componentPools[componentId])};

    if (entityId >= componentPool->GetSize()) {
      componentPool->Resize(_numEntities);
    }

    // Create a new component object of type T,
    // and forward the various paramenters to the constructor
    T newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);
    // change the component signature of the entity and set the component id on
    // the bitset to 1
    _entityComponentSignatures[entityId].set(componentId);

    Logger::Log("Component id = " + std::to_string(componentId) +
                " was added to entity id " + std::to_string(entityId));
  }

  template <typename T> void RemoveComponent(Entity entity) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};
    _entityComponentSignatures[entityId].set(componentId, false);

    Logger::Log("Component id = " + std::to_string(componentId) +
                " was removed from entity id " + std::to_string(entityId));
  }

  template <typename T> bool HasComponent(Entity entity) {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};

    return _entityComponentSignatures[entityId].test(componentId);
  }

  template <typename T> T &GetComponent(Entity entity) const {
    const auto componentId{Component<T>::GetId()};
    const auto entityId{entity.GetId()};
    const auto component{
        std::static_pointer_cast<Pool<T>>(_componentPools[componentId])};
    return component->Get(entityId);
  }

private:
  EntityManager() = default;

  void _addEntityToSystem(Entity entity);
  void _removeEntityFromSystems(Entity entity);

  unsigned int _numEntities{};

  // Each pool contains all the data for a certain component type.
  // Vector index = component type id
  // Pool index = entity id
  std::vector<std::shared_ptr<IPool>> _componentPools{};

  // Vector of component signatures per entity, saying which component is turned
  // "on" for a given entity Vector index = entity id
  std::vector<Signature> _entityComponentSignatures{};

  // Map of active systems
  // [Map key = system type_id]
  std::unordered_map<std::type_index, std::shared_ptr<System>> _systems{};

  std::set<Entity> _entitiesToBeCreated{};
  std::set<Entity> _entitiesToBeKilled{};

  std::deque<int> _freeIds{};
};

#endif
