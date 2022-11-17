#pragma once

#include "attribute.hpp"
#include "jvm/bytecode/simple/forward.hpp"
#include "jvm/types/class/class_field.hpp"
#include "jvm/types/const_pool.hpp"

namespace jvm {
    struct Method: public jvm::ClassField {
        bytecode::Instructions code{};
        Method(jvm::AccessFlags acFlags, jvm::ConstRef<std::string> fieldName, Index typeDescriptorIndex, Attributes fieldAttributes, bytecode::Instructions code):
              jvm::ClassField{.flags = acFlags, .name = fieldName, .descriptorIndex = typeDescriptorIndex, .attributes = std::move(fieldAttributes)}, code(std::move(code)) {
            // TODO: assign mandatory attributes
        }
    };

    using Methods = std::vector<jvm::Method>;
}

