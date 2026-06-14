#ifndef ENTITY_H
#define ENTITY_H

#include "../Logger/Logger.h"
#include "Component.h"
#include <bitset>
#include <cassert>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

class Entity {
public:
  bool operator==(const Entity &other) const { return id == other.GetId(); }

  bool operator!=(const Entity &other) const { return id != other.GetId(); }

  bool operator>(const Entity &other) const { return id > other.GetId(); }

  bool operator<(const Entity &other) const { return id < other.GetId(); }

  Entity &operator=(const Entity &other) = default;

  Entity(int id) : id(id) {};

  int GetId() const;

private:
  int id{};
};

#endif
