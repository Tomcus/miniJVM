#pragma once

#include "jvm/types/class/class_field.hpp"

namespace jvm {
    struct Field : public jvm::ClassField {
        Field(jvm::AccessFlags acFlags, jvm::ConstRef<std::string> fieldName,
              jvm::ConstRef<std::string> typeDescriptor, Attributes fieldAttributes);
    };

    using Fields = std::vector<jvm::Field>;
}
