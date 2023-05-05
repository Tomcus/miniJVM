#pragma once

#include "jvm/execution/instructions/forward.hpp"

namespace jvm::op {

struct nop {
    static constexpr std::uint8_t OP_CODE = 0x00;
    [[nodiscard]] static nop parse();
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;
};

}
