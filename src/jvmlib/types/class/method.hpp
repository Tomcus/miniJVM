#ifndef MINI_JVM_TYPES_CLASS_METHOD_HPP
#define MINI_JVM_TYPES_CLASS_METHOD_HPP

#include "attribute.hpp"

namespace jvm {
    struct Method {
        std::uint16_t flags;
        Index nameIndex;
        Index descriptorIndex;
        Attributes attributes;
    };

    using Methods = std::vector<jvm::Method>;
}

#endif//MINI_JVM_TYPES_CLASS_METHOD_HPP