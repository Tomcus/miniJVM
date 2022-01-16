#ifndef MINI_JVM_TYPES_CONST_HPP
#define MINI_JVM_TYPES_CONST_HPP

#include "basic.hpp"
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace jvm {

using ConstPollIndex = std::uint16_t;

enum class tag : std::uint8_t {
    String = 0x01,
    Name = 0x07,
    StringRef = 0x08,
    Class = 0x09,
    NameAndType = 0x0a,
    NameAndDesc = 0x0c
};

struct ConstPoolValue {
    tag valTag;
    ConstPollIndex nameIndex;
    ConstPollIndex classIndex;
    ConstPollIndex nameAndTypeIndex;
    ConstPollIndex stringIndex;
    ConstPollIndex descIndex;
    std::string string;
};

}

#endif//MINI_JVM_TYPES_CONST_HPP