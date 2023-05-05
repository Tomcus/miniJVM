#pragma once

#include "jvm/execution/instructions/forward.hpp"
#include "jvm/types/basic.hpp"

namespace jvm::op {

struct ldc {
    static constexpr std::uint8_t OP_CODE = 0x12;
    static ldc parse(jvm::Bytes& bytes);
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;
    jvm::Index cpIndex;
};

struct ldc_w {
    static constexpr std::uint8_t OP_CODE = 0x13;
    static ldc_w parse(jvm::Bytes& bytes);
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;
    jvm::Index cpIndex;
};

struct ldc2_w {
    static constexpr std::uint8_t OP_CODE = 0x14;
    static ldc2_w parse(jvm::Bytes& bytes);
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;
    jvm::Index cpIndex;
};

}
