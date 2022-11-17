#pragma once

#include <string_view>

#include "forward.hpp"

namespace jvm::bytecode {

class BaseInstruction {
public:
    static std::string_view name; 
    
    BaseInstruction() = default;
    virtual ~BaseInstruction() = default;
    
    virtual void execute(JVM_INSTRUCTION_INVOKE_PARAMS) = 0;
};

class nop: public BaseInstruction {
    nop() = default;
    virtual ~nop() = default;

    void execute(JVM_INSTRUCTION_INVOKE_PARAMS) final;
};

}
