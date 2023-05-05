#include "jvm/execution/instructions/misc.hpp"

using namespace jvm::op;

nop nop::parse() {
    return {};
}

void nop::operator()(INSTRUCTION_INVOKE_FUNCTION) const { }
