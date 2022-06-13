#ifndef MINI_JVM_TYPES_CLASS_FIELD_HPP
#define MINI_JVM_TYPES_CLASS_FIELD_HPP

#include "jvm/types/class/class_field.hpp"

namespace jvm {
    struct Field : public jvm::ClassField {
        Field(jvm::AccessFlags acFlags, jvm::ConstRef<std::string> fieldName, Index typeDescriptorIndex, Attributes fieldAttributes):
              jvm::ClassField{.flags = acFlags, .name = fieldName, .descriptorIndex = typeDescriptorIndex, .attributes = std::move(fieldAttributes)} {
            // TODO: assign mandatory attributes
        }
    };

    using Fields = std::vector<jvm::Field>;
}

#endif//MINI_JVM_TYPES_CLASS_FIELD_HPP