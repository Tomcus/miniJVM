#include "jvm/execution/instructions/stack_load_const_pool.hpp"

using namespace jvm::op;

ldc ldc::parse(jvm::Bytes& bytes) {
    jvm::Index index = bytes[0];
    bytes = bytes.subspan(1);
    return {
        .cpIndex = index
    };
}

ldc_w ldc_w::parse(jvm::Bytes& bytes) {
    jvm::Index index = (bytes[0] << 8) | bytes[1];
    bytes = bytes.subspan(2);
    return {
        .cpIndex = index
    };
}

ldc2_w ldc2_w::parse(jvm::Bytes& bytes) {
    jvm::Index index = (bytes[0] << 8) | bytes[1];
    bytes = bytes.subspan(2);
    return {
        .cpIndex = index
    };
}

void ldc::operator()(INSTRUCTION_INVOKE_FUNCTION) const {
    // TODO
}

void ldc_w::operator()(INSTRUCTION_INVOKE_FUNCTION) const {
    // TODO
}

void ldc2_w::operator()(INSTRUCTION_INVOKE_FUNCTION) const {
    // TODO
}
