#include <jvm/bytecode/simple/basic.hpp>

using namespace jvm::bytecode;

void nop::execute(JVM_INSTRUCTION_INVOKE_PARAMS) {
    return;
}
