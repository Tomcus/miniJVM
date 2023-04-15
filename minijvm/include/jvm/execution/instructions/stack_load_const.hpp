#pragma once

#include <nonstd/expected.hpp>

#include "jvm/execution/instructions/forward.hpp"
#include "jvm/execution/stack.hpp"

namespace jvm::op {

namespace impl {
    
template<typename Self, typename PrimitiveType, PrimitiveType VALUE>
struct StackPushConstant {
    static Self parse() {
        return Self{};
    }

    void operator()(INSTRUCTION_INVOKE_FUNCTION) {
        stack.push(VALUE);
    }
};

}

struct iconst_m1: public impl::StackPushConstant<iconst_m1, int, -1> {
    static constexpr std::uint8_t OP_CODE = 0x02;
};
struct iconst_0: public impl::StackPushConstant<iconst_0, int, 0> {
    static constexpr std::uint8_t OP_CODE = 0x03;
};
struct iconst_1: public impl::StackPushConstant<iconst_1, int, 1> {
    static constexpr std::uint8_t OP_CODE = 0x04;
};
struct iconst_2: public impl::StackPushConstant<iconst_2, int, 2> {
    static constexpr std::uint8_t OP_CODE = 0x05;
};
struct iconst_3: public impl::StackPushConstant<iconst_3, int, 3> {
    static constexpr std::uint8_t OP_CODE = 0x06;
};
struct iconst_4: public impl::StackPushConstant<iconst_4, int, 4> {
    static constexpr std::uint8_t OP_CODE = 0x07;
};
struct iconst_5: public impl::StackPushConstant<iconst_5, int, 5> {
    static constexpr std::uint8_t OP_CODE = 0x08;
};

}
