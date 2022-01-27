#ifndef MINI_JVM_TYPES_BASIC_HPP
#define MINI_JVM_TYPES_BASIC_HPP

#include <cstdint>
#include <variant>
#include <vector>
#include <string>

namespace jvm {

using Byte = std::uint8_t;
using PrimitiveInteger = std::variant<std::int32_t, std::int64_t>;
using PrimitiveFloat = std::variant<float, double>;
using PrimitiveType = std::variant<PrimitiveFloat, PrimitiveInteger>;
using PrimitiveArray = std::variant<std::vector<std::int32_t>, std::vector<float>, std::vector<std::int64_t>, std::vector<double>>;
using BasicType = std::variant<PrimitiveType, std::string>;
using BasicArray = std::variant<PrimitiveArray, std::vector<std::string>>;

}

#endif//MINI_JVM_TYPES_BASIC_HPP