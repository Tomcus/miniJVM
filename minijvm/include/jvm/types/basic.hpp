#ifndef MINI_JVM_TYPES_BASIC_HPP
#define MINI_JVM_TYPES_BASIC_HPP

#include <cstdint>
#include <variant>
#include <vector>
#include <string>

#include <nonstd/expected.hpp>

#include "jvm/types/class/base.hpp"

namespace jvm {

using PrimitiveInteger = std::variant<std::int32_t, std::int64_t>;
using PrimitiveFloat = std::variant<float, double>;
using PrimitiveType = std::variant<PrimitiveFloat, PrimitiveInteger>;
using PrimitiveArray = std::variant<std::vector<std::int32_t>, std::vector<float>, std::vector<std::int64_t>, std::vector<double>>;
using BasicType = std::variant<PrimitiveType, std::string>;
using BasicArray = std::variant<PrimitiveArray, std::vector<std::string>>;

struct ParsingError {
    std::string message;
    static ParsingError cant_read_from_istream(std::istream&, std::string_view);
    static ParsingError unsuported_type();
};

}

#define PROPAGATE_ERROR(exptected) if (!exptected) return nonstd::make_unexpected(exptected.error())

#endif//MINI_JVM_TYPES_BASIC_HPP
