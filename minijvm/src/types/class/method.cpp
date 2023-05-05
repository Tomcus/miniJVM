#include "jvm/types/class/method.hpp"

#include <fmtlog/fmtlog.h>

jvm::Method::Method(
        jvm::AccessFlags acFlags,
        jvm::ConstRef<std::string> fieldName,
        jvm::ConstRef<std::string> methodTypeDescriptor,
        Attributes fieldAttributes): jvm::ClassField{
            .flags = acFlags,
            .name = fieldName,
            .typeDescriptor = methodTypeDescriptor,
            .attributes = std::move(fieldAttributes)
        }, code{}
{
    auto codeBytes = getAndRemoveAttributeData("Code");
    auto codeBytesSpan = std::span(codeBytes.data(), codeBytes.size());

    auto res = jvm::parseInstructions(codeBytesSpan);
    if (!res) {
        loge("Method parsing error: {}", res.error().message);
        fmtlog::poll();
    }

    code = *res;
}
