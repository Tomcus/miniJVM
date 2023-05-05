#include "jvm/execution/instructions/stack_load_local.hpp"

using namespace jvm::op;

aload aload::parse(jvm::Bytes& bytes) {
    jvm::Index index = (bytes[0] << 8) + bytes[1];
    bytes = bytes.subspan(2);
    return {.localIndex = index};
}

void aload::operator()(INSTRUCTION_INVOKE_FUNCTION) const {
    // TODO
}

iload iload::parse(jvm::Bytes& bytes) {
    jvm::Index index = (bytes[0] << 8) + bytes[1];
    bytes = bytes.subspan(2);
    return {.localIndex = index};
}

void iload::operator()(INSTRUCTION_INVOKE_FUNCTION) const {
    // TODO
}
