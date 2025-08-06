#pragma once

#include "../Logger/Logger.h"
#include "IPool.h"
#include "System.h"
#include "Types.h"
#include <bitset>
#include <cassert>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

class Entity;

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

  void AddEntityToSystem(Entity entity);

  void DeleteEntity(Entity *entity);

  template <typename TSystem, typename... TArgs>
  void AddSystem(TArgs &&...args) {

    std::shared_ptr<TSystem> newSystem{
        std::make_shared<TSystem>(std::forward<TArgs>(args)...)};
    std::type_index index{std::type_index(typeid(TSystem))};

    systems.insert(std::make_pair(index, newSystem));
  }

  template <typename TSystem> void RemoveSystem() {
    const auto system{systems.find(std::type_index(typeid(TSystem)))};
    systems.erase(system);
  }

  template <typename TSystem> bool HasSystem() const {
    const auto index{std::type_index(typeid(TSystem))};
    return systems.find(index) != systems.end();
  }

  template <typename TSystem> TSystem &GetSystem() const {
    auto system{systems.find(std::type_index(typeid(TSystem)))};
    return *(std::static_pointer_cast<TSystem>(system->second));
  }

  unsigned int numEntities{};

  // Each pool contains all the data for a certain component type.
  // Vector index = component type id
  // Pool index = entity id
  std::vector<std::shared_ptr<IPool>> componentPools{};

  // Vector of component signatures per entity, saying which component is turned
  // "on" for a given entity Vector index = entity id
  std::vector<Signature> entityComponentSignatures{};

  // Map of active systems
  // [Map key = system type_id]
  std::unordered_map<std::type_index, std::shared_ptr<System>> systems{};

  std::set<Entity> entitiesToBeCreated{};
  std::set<Entity> entitiesToBeKilled{};

private:
  EntityManager() = default;
};
