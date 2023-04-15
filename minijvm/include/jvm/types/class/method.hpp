#ifndef MINI_JVM_TYPES_CLASS_METHOD_HPP
#define MINI_JVM_TYPES_CLASS_METHOD_HPP

#include "jvm/types/class/class_field.hpp"
#include "jvm/execution/instructions.hpp"

namespace jvm {

struct Method : public jvm::ClassField {
public:
    Method(jvm::AccessFlags acFlags,
           jvm::ConstRef<std::string> fieldName,
           jvm::ConstRef<std::string> typeDescriptor,
           Attributes fieldAttributes);
protected:
    Instructions code;
};

using Methods = std::vector<jvm::Method>;

}

#endif//MINI_JVM_TYPES_CLASS_METHOD_HPP
