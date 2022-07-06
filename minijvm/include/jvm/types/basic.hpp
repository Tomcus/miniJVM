#ifndef MINI_JVM_TYPES_BASIC_HPP
#define MINI_JVM_TYPES_BASIC_HPP

#include <cstdint>
#include <variant>
#include <vector>
#include <string>

#include "jvm/types/class/base.hpp"
#include "jvm/utils/ref.hpp"

namespace jvm {

class ClassInstanceBase;

#define PRIMITE_INTEGERS std::int32_t, std::int64_t
using PrimitiveInteger = std::variant<PRIMITE_INTEGERS>;
#define PRIMITE_FLOATS float, double
using PrimitiveFloat = std::variant<PRIMITE_FLOATS>;
using PrimitiveType = std::variant<PRIMITE_INTEGERS, PRIMITE_FLOATS, bool>;
using ComplexType = std::variant<std::monostate, Ref<ClassInstanceBase>>;
using VariableType = std::variant<PrimitiveType, ComplexType>;

}

#endif//MINI_JVM_TYPES_BASIC_HPP
