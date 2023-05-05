#pragma once

#include <nonstd/expected.hpp>

#include "jvm/execution/instructions/forward.hpp"

namespace jvm::op {

namespace impl {
    
template<typename Self, typename PrimitiveType, PrimitiveType VALUE>
struct StackPushConstant {
    [[nodiscard]] static Self parse() {
        return Self{};
    }

    void operator()(INSTRUCTION_INVOKE_FUNCTION) const {
        stack.push(VALUE);
    }
};

template<typename Self, std::size_t TypeSize>
struct PushSmallerTypeConstant {
    int toPush;
    [[nodiscard]] static Self parse(jvm::Bytes& bytes) {
        auto data = bytes.subspan(0, TypeSize);
        bytes = bytes.subspan(TypeSize);
        int toPush = 0;
        for (auto byte: data) {
            toPush = (toPush << 8) + byte;
        }
        return Self{toPush};
    }

    void operator()(INSTRUCTION_INVOKE_FUNCTION) const {
        stack.push(toPush);
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

struct lconst_0: public impl::StackPushConstant<lconst_0, std::int64_t, 0L> {
    static constexpr std::uint8_t OP_CODE = 0x09;
};

struct lconst_1: public impl::StackPushConstant<lconst_1, std::int64_t, 1L> {
    static constexpr std::uint8_t OP_CODE = 0x0a;
};

struct aconst_null {
    static constexpr std::uint8_t OP_CODE = 0x01;
    [[nodiscard]] static aconst_null parse();
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;
};

struct bipush: impl::PushSmallerTypeConstant<bipush, 1> {
    static constexpr std::uint8_t OP_CODE = 0x10;
};

struct sipush: impl::PushSmallerTypeConstant<sipush, 2> {
    static constexpr std::uint8_t OP_CODE = 0x11;
};

}
