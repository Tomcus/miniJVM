#pragma once

#include "jvm/execution/instructions/forward.hpp"
#include "jvm/types/class/base.hpp"

namespace jvm::op {

struct invokespecial {
    [[nodiscard]] static invokespecial parse(jvm::Bytes& bytes);
    static constexpr std::uint8_t OP_CODE = 0xb7;
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;

    jvm::Index cpIndex;
};

}
