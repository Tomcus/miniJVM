#pragma once

#include <utility>

#include "jvm/execution/instructions/forward.hpp"

namespace jvm::op {
namespace impl {

template<typename Self, typename IntType>
struct BinaryOperator {
    [[nodiscard]] static Self parse() {
        return {};
    }

protected:
    std::pair<IntType, IntType> popOperands(jvm::Stack& stack) const {
        auto a = std::get<IntType>(stack.top());
        stack.pop();
        auto b = std::get<IntType>(stack.top());
        stack.pop();
        return {a, b};
    }
};

template<typename Self, typename IntType>
struct Add: public BinaryOperator<Self, IntType> {
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const {
        auto [a, b] = this->popOperands(stack);
        stack.push(a + b);
    }
};

}

struct iadd: public impl::Add<iadd, int> {
    static constexpr std::uint8_t OP_CODE = 0x60;
};

}
