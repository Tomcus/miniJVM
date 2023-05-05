#pragma once

#include "jvm/execution/instructions/forward.hpp"
#include "jvm/types/class/base.hpp"

namespace jvm::op {
namespace impl {

template<typename Self, std::size_t Index>
struct LoadLocalToStack {
    [[nodiscard]] static Self parse() {
        return Self{};
    }
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const {
        // TODO
    }
};

}

struct aload_0: public impl::LoadLocalToStack<aload_0, 0> {
    static constexpr std::uint8_t OP_CODE = 0x2a;
};

struct aload_1: public impl::LoadLocalToStack<aload_1, 1> {
    static constexpr std::uint8_t OP_CODE = 0x2b;
};

struct aload_2: public impl::LoadLocalToStack<aload_2, 2> {
    static constexpr std::uint8_t OP_CODE = 0x2c;
};

struct aload_3: public impl::LoadLocalToStack<aload_3, 3> {
    static constexpr std::uint8_t OP_CODE = 0x2d;
};

struct iload_0: public impl::LoadLocalToStack<iload_0, 0> {
    static constexpr std::uint8_t OP_CODE = 0x1a;
};

struct iload_1: public impl::LoadLocalToStack<iload_1, 1> {
    static constexpr std::uint8_t OP_CODE = 0x1b;
};

struct iload_2: public impl::LoadLocalToStack<iload_2, 2> {
    static constexpr std::uint8_t OP_CODE = 0x1c;
};

struct iload_3: public impl::LoadLocalToStack<iload_3, 3> {
    static constexpr std::uint8_t OP_CODE = 0x1d;
};

struct aload {
    [[nodiscard]] static aload parse(jvm::Bytes& bytes);
    static constexpr std::uint8_t OP_CODE = 0x19;
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;
    jvm::Index localIndex;
};

struct iload {
    [[nodiscard]] static iload parse(jvm::Bytes& bytes);
    static constexpr std::uint8_t OP_CODE = 0x15;
    void operator()(INSTRUCTION_INVOKE_FUNCTION) const;
    jvm::Index localIndex;
};

}
