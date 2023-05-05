#include "jvm/execution/instructions/return.hpp"

using namespace jvm::op;

ret ret::parse() {
    return {};
}

void ret::operator()(INSTRUCTION_INVOKE_FUNCTION) const {
    // TODO
}
