#include "Entity.h"
#include "EntityManager.h"

int Entity::GetId() const { return id; }

EntityManager& Entity::_getEntityManager() const {
  return EntityManager::Get();
}
