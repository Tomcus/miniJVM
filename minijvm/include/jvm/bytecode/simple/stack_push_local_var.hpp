#pragma once

#include "basic.hpp"
#include <variant>

namespace jvm::bytecode {

template<typename VariableType>
class StackPushLocalValueInstruction: public BaseInstruction {
public:
    StackPushLocalValueInstruction(std::uint16_t index): index(index) { }
    virtual ~StackPushLocalValueInstruction() = default;

    void execute(JVM_INSTRUCTION_INVOKE_PARAMS) final {
        if (index < local.size() && std::holds_alternative<VariableType>(local[index])) {
            stack.push(std::get<VariableType>(local[0]));
        }
    }
protected:
    std::uint16_t index;
};


class iload: StackPushLocalValueInstruction<std::int32_t> {
public:
    iload(std::uint16_t index): StackPushLocalValueInstruction<std::int32_t>(index) { }
};

class lload: StackPushLocalValueInstruction<std::int64_t> {
public:
    lload(std::uint16_t index): StackPushLocalValueInstruction<std::int64_t>(index) { }
};

class fload: StackPushLocalValueInstruction<float> {
public:
    fload(std::uint16_t index): StackPushLocalValueInstruction<float>(index) { }
};

class dload: StackPushLocalValueInstruction<double> {
public:
    dload(std::uint16_t index): StackPushLocalValueInstruction<double>(index) { }
};


class iload_0: public iload {
public:
    iload_0(): iload(0) { }
};

class iload_1: public iload {
public:
    iload_1(): iload(1) { }
};

class iload_2: public iload {
public:
    iload_2(): iload(2) { }
};

class iload_3: public iload {
public:
    iload_3(): iload(3) { }
};

class lload_0: public lload {
public:
    lload_0(): lload(0) { }
};

class lload_1: public lload {
public:
    lload_1(): lload(1) { }
};

class lload_2: public lload {
public:
    lload_2(): lload(2) { }
};

class lload_3: public lload {
public:
    lload_3(): lload(3) { }
};

class fload_0: public fload {
public:
    fload_0(): fload(0) { }
};

class fload_1: public fload {
public:
    fload_1(): fload(1) { }
};

class fload_2: public fload {
public:
    fload_2(): fload(2) { }
};

class fload_3: public fload {
public:
    fload_3(): fload(3) { }
};

class dload_0: public dload {
public:
    dload_0(): dload(0) { }
};

class dload_1: public dload {
public:
    dload_1(): dload(1) { }
};

class dload_2: public dload {
public:
    dload_2(): dload(2) { }
};

class dload_3: public dload {
public:
    dload_3(): dload(3) { }
};

}
