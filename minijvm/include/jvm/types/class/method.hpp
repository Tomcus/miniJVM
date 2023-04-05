#ifndef MINI_JVM_TYPES_CLASS_METHOD_HPP
#define MINI_JVM_TYPES_CLASS_METHOD_HPP

#include "jvm/types/class/class_field.hpp"

namespace jvm {

struct Method : public jvm::ClassField {
    Method(jvm::AccessFlags acFlags,
           jvm::ConstRef<std::string> fieldName,
           Index typeDescriptorIndex,
           Attributes fieldAttributes);
};

using Methods = std::vector<jvm::Method>;

}

#endif//MINI_JVM_TYPES_CLASS_METHOD_HPP
