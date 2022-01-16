#ifndef MINI_JVM_TYPES_CLASS_HPP
#define MINI_JVM_TYPES_CLASS_HPP

#include "basic.hpp"
#include "const.hpp"

#include <cstdint>
#include <vector>

namespace jvm {

struct ver16 {
    std::uint16_t major;
    std::uint16_t minor;
};

struct Attribute {
    ConstPollIndex attrNameIndex;
    std::vector<byte> data;
};

using Attributes = std::vector<Attribute>;

struct Field {
    std::uint16_t flags;
    ConstPollIndex nameIndex;
    ConstPollIndex descriptorIndex;
    Attributes attributes;
};

using Fields = std::vector<jvm::Field>;
using Interfaces = std::vector<ConstPollIndex>;
using ConstPool = std::vector<ConstPoolValue>;

struct Class {
    ver16 version;
    ConstPool constpool;
    AccessFlags accessFlags;
    ConstPollIndex thisClass;
    ConstPollIndex superClass;
    Interfaces interfaces;
    Fields fields;
    Fields methods;
    Attributes attributes;
};

}

#endif//MINI_JVM_TYPES_CLASS_HPP