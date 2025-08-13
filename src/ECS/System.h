#ifndef SYSYEM_H
#define SYSYEM_H

#include <vector>
#include "Types.h"
#include "Component.h"

class Entity;

class System {
public:
  System() = default;
  ~System() = default;

  void AddEntity(Entity entity);
  void RemoveEntity(Entity entity);
  std::vector<Entity> GetEntities() const;
  const Signature &GetComponentSignature() const;

  template <typename TComponent> void RequireComponent();

private:
  Signature componentSignature{};
  std::vector<Entity> entities{};
};

template <typename TComponent> void System::RequireComponent() {
  const auto componentId{Component<TComponent>::GetId()};
  componentSignature.set(componentId);
}

#endif
