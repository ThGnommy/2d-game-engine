#pragma once

#include "iostream"
#include <vector>

class IPool {

  //  -------->
  //  componentsPools
  //  -------------------------
  //  |   |   |   |   |   |   |   P   |
  //  |---|---|---|---|---|---|   o   |
  //  |   |   |   |   |   |   |   o   |
  //  |---|---|---|---|---|---|   l   |
  //  |   |   |   |   |   |   |   *   V
  //  |---|---|---|---|---|---|
  //  |   |   |   |   |   |   |
  //  |---|---|---|---|---|---|
  //  |   |   |   |   |   |   |  > Entity
  //  |---|---|---|---|---|---|
  //  |   |   |   |   |   |   |  > Entity
  //  -------------------------

public:
  virtual ~IPool() {};
};

// This is a vector of components's entity.
template <typename T> class Pool : public IPool {
private:
  // data rapresents an array of components type T
  // e.g TransformComponent
  std::vector<T> data{};

public:
  Pool(int size = 100) { data.resize(size); }

  virtual ~Pool() = default;

  bool isEmpty() const { return data.empty(); }

  int GetSize() const { return data.size(); }

  void Resize(const int n) { data.resize(n); }

  void Clear() { data.clear(); }

  void Add(const T object) { data.push_back(object); }

  void Set(int index, T object) { data[index] = object; }

  const T &Get(const unsigned int index) const {
    return static_cast<T &>(data[index]);
  }

  T &operator[](unsigned int index) { return data[index]; }
};
