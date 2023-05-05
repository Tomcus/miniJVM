#include "jvm/execution/instructions/invoke.hpp"

using namespace jvm::op;

invokespecial invokespecial::parse(jvm::Bytes& bytes) {
    jvm::Index index = (bytes[0] << 8) + bytes[1];
    bytes = bytes.subspan(2);
    return {.cpIndex = index};
}

void invokespecial::operator()(INSTRUCTION_INVOKE_FUNCTION) const {
    // TODO
}
