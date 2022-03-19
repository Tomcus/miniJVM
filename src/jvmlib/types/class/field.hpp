#ifndef MINI_JVM_TYPES_CLASS_FIELD_HPP
#define MINI_JVM_TYPES_CLASS_FIELD_HPP

#include "attribute.hpp"

namespace jvm {
    struct Field {
        std::uint16_t flags;
        Index nameIndex;
        Index descriptorIndex;
        Attributes attributes;
    };

    using Fields = std::vector<jvm::Field>;
}

#endif//MINI_JVM_TYPES_CLASS_FIELD_HPP