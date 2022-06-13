#ifndef MINI_JVM_TYPES_CLASS_CLASS_FIELD_HPP
#define MINI_JVM_TYPES_CLASS_CLASS_FIELD_HPP

#include "jvm/types/class/attribute.hpp"
#include "jvm/types/class/base.hpp"
#include "jvm/types/const_pool.hpp"

namespace jvm {
    struct ClassField {
        jvm::AccessFlags flags;
        jvm::ConstRef<std::string> name;
        Index descriptorIndex;
        Attributes attributes;
    };
}

#endif//MINI_JVM_TYPES_CLASS_CLASS_FIELD_HPP