#pragma once

#include <bitset>

#define assertm(exp, msg) assert((void(msg), exp))
constexpr unsigned int MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;
