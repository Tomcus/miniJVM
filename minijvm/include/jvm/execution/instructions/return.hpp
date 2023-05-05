#pragma once

#include "jvm/execution/instructions/forward.hpp"

namespace jvm::op {

namespace impl {

template<typename Self, typename Type>
struct TypedReturn {
    [[nodiscard]] static Self parse() {
        return {};
    }

    void operator()(INSTRUCTION_INVOKE_FUNCTION) const {
        // TODO
    }
};

}

struct ret { // can't use return, beacuse it is a c++ keyword
    [[nodiscard]] static ret parse();
    static constexpr std::uint8_t OP_CODE = 0xb1;
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;
};

struct ireturn: public impl::TypedReturn<ireturn, int> {
    static constexpr std::uint8_t OP_CODE = 0xac;
};

}
