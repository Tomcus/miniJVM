#pragma once

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

