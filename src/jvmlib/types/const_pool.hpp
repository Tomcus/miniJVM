#ifndef MINI_JVM_TYPES_CONST_POOL_HPP
#define MINI_JVM_TYPES_CONST_POOL_HPP

#include "basic.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <stdexcept>
#include <typeinfo>

#include <fmt/format.h>

namespace jvm::ConstPool {

using Index = std::uint16_t;

enum class Tag : std::uint8_t {
    String = 0x01,
    Integer = 0x03,
    Float = 0x04,
    Long = 0x05,
    Double = 0x06,
    Class = 0x07,
    StringRef = 0x08,
    FieldRef = 0x09,
    MethodRef = 0x0a,
    InterfaceMethodRef = 0x0b,
    NameAndType = 0x0c,
    MethodHandle = 0x0f,
    MethodType = 0x10,
    Dynamic = 0x11,
    InvokeDynamic = 0x12,
    Module = 0x13,
    Package = 0x14
};

struct NameAndType {
    Index name;
    Index type;
};

struct MethodRef {
    Index classInfo;
    Index nameAndType;
};

struct ClassInfo {
    Index name;
};

using Value = std::variant<MethodRef, ClassInfo, NameAndType, BasicType>;

class Error: public std::runtime_error {
public:
    Error(const std::string& message): runtime_error(message) { }
};

template<typename Error>
inline void check(bool condition, const std::string_view message = "") {
    if (!condition) {
        if (message.empty()) {
            throw Error{fmt::format("Check failed with message: NO MESSAGE PROVIDED (err_type: {})", typeid(Error).name())};
        } else {
            throw Error{fmt::format("Check failed with message: {}", message)};
        }
    }
}

}

#endif//MINI_JVM_TYPES_CONST_POOL_HPP