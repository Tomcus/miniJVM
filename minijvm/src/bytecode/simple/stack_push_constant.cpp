#include <jvm/bytecode/simple/stack_push_constant.hpp>
#include <variant>

using namespace jvm::bytecode;

void aconst_null::execute(JVM_INSTRUCTION_INVOKE_PARAMS) {
    stack.push(std::monostate{});
}
