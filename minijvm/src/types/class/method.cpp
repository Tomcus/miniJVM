#include "jvm/types/class/method.hpp"

jvm::Method::Method(
        jvm::AccessFlags acFlags,
        jvm::ConstRef<std::string> fieldName,
        jvm::ConstRef<std::string> methodTypeDescriptor,
        Attributes fieldAttributes): jvm::ClassField{
    .flags = acFlags,
    .name = fieldName,
    .typeDescriptor = methodTypeDescriptor,
    .attributes = std::move(fieldAttributes)
} {
    /*auto codeBytes = getAndRemoveAttributeData("Code");
    auto codeBytesSpan = std::span(codeBytes.data(), codeBytes.size());
    code = *jvm::parseInstructions(codeBytesSpan);*/
}
