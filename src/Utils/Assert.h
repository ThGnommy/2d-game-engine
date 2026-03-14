#pragma once
#include <cassert>

#define assert_m(condition, message)                                           \
  if (!(condition)) {                                                          \
    Logger::Err(std::string("Assertion failed: ") + message);                               \
    assert(condition);                                                         \
  }
