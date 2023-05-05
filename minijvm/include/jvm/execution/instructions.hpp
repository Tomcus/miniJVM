#pragma once

#include <span>
#include <vector>
#include <variant>
#include <cstdint>

#include <nonstd/expected.hpp>

#include "jvm/execution/instructions/int_ops.hpp"
#include "jvm/execution/instructions/invoke.hpp"
#include "jvm/execution/instructions/misc.hpp"
#include "jvm/execution/instructions/return.hpp"
#include "jvm/execution/instructions/stack_load_const.hpp"
#include "jvm/execution/instructions/stack_load_const_pool.hpp"
#include "jvm/execution/instructions/stack_load_local.hpp"
#include "jvm/types/basic.hpp"

namespace jvm {

using Instruction = std::variant<
    jvm::op::nop,
    jvm::op::aconst_null,
    jvm::op::iconst_0,
    jvm::op::iconst_1,
    jvm::op::iconst_2,
    jvm::op::iconst_3,
    jvm::op::iconst_4,
    jvm::op::iconst_5,
    jvm::op::iconst_m1,
    jvm::op::lconst_0,
    jvm::op::lconst_1,
    jvm::op::bipush,
    jvm::op::sipush,
    jvm::op::ldc,
    jvm::op::ldc_w,
    jvm::op::ldc2_w,
    jvm::op::aload_0,
    jvm::op::aload_1,
    jvm::op::aload_2,
    jvm::op::aload_3,
    jvm::op::aload,
    jvm::op::ret,
    jvm::op::ireturn,
    jvm::op::invokespecial,
    jvm::op::iload,
    jvm::op::iload_0,
    jvm::op::iload_1,
    jvm::op::iload_2,
    jvm::op::iload_3,
    jvm::op::iadd
>;
using Instructions = std::vector<Instruction>;

[[nodiscard]] nonstd::expected<Instruction, ParsingError> parseInstruction(std::span<std::uint8_t>& bytes);
[[nodiscard]] nonstd::expected<Instructions, ParsingError> parseInstructions(std::span<std::uint8_t>& bytes);
}
