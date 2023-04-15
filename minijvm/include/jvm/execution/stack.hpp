#pragma once

#include <variant>
#include <stack>
#include <cstdint>

namespace jvm {

using StackItem = std::variant<std::int32_t, std::int64_t, float, double>;
using Stack = std::stack<StackItem>;

}
