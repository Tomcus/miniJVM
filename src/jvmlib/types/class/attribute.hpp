#ifndef MINI_JVM_TYPES_CLASS_ATTRIBUTE_HPP
#define MINI_JVM_TYPES_CLASS_ATTRIBUTE_HPP

#include <vector>
#include "base.hpp"

namespace jvm {
    struct Attribute {
        jvm::Index attrNameIndex;
        std::vector<Byte> data;
    };

    using Attributes = std::vector<Attribute>;
}

#endif//MINI_JVM_TYPES_CLASS_ATTRIBUTE_HPP