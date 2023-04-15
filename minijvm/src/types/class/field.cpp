#include "jvm/types/class/field.hpp"

using namespace jvm;

Field::Field(jvm::AccessFlags acFlags,
             jvm::ConstRef<std::string> fieldName,
             jvm::ConstRef<std::string> fieldTypeDescriptor,
             Attributes fieldAttributes): ClassField {
                 .flags = acFlags,
                 .name = fieldName,
                 .typeDescriptor = fieldTypeDescriptor,
                 .attributes = std::move(fieldAttributes)
             }
{
    // TODO: Parse required attributes                
}
