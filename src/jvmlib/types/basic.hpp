#ifndef MINI_JVM_TYPES_BASIC_HPP
#define MINI_JVM_TYPES_BASIC_HPP

#include <cstdint>

namespace jvm {
    using byte = std::uint8_t;

enum class AccessFlags: std::uint16_t {
    ACC_PUBLIC = 0x001,
    ACC_FINAL = 0x010,
    ACC_SUPER = 0x020,
    ACC_INTERFACE = 0x200,
    ACC_ABSTRACT = 0x400
};

}

#endif//MINI_JVM_TYPES_BASIC_HPP