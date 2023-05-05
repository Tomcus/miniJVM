#include "jvm/execution/instructions/stack_load_const.hpp"

using namespace jvm::op;

aconst_null aconst_null::parse() {
    return aconst_null{};
}

void aconst_null::operator()(INSTRUCTION_INVOKE_FUNCTION) const {
    stack.push(std::monostate{});
}
