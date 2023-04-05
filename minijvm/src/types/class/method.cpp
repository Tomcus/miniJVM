#include "jvm/types/class/method.hpp"

jvm::Method::Method(
        jvm::AccessFlags acFlags,
        jvm::ConstRef<std::string> fieldName,
        Index typeDescriptorIndex,
        Attributes fieldAttributes): jvm::ClassField{
    .flags = acFlags,
    .name = fieldName,
    .descriptorIndex = typeDescriptorIndex,
    .attributes = std::move(fieldAttributes)
} {
    // TODO: assign mandatory attributes
}
