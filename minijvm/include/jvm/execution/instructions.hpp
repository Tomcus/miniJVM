#pragma once

#include <span>
#include <vector>
#include <variant>
#include <cstdint>

#include <nonstd/expected.hpp>

#include "jvm/execution/instructions/stack_load_const.hpp"
#include "jvm/types/basic.hpp"

namespace jvm {

using Instruction = std::variant<
    std::monostate,
    jvm::op::iconst_0,
    jvm::op::iconst_1,
    jvm::op::iconst_2,
    jvm::op::iconst_3,
    jvm::op::iconst_4,
    jvm::op::iconst_5,
    jvm::op::iconst_m1
>;
using Instructions = std::vector<Instruction>;

[[nodiscard]] nonstd::expected<Instruction, ParsingError> parseInstruction(std::span<std::uint8_t>& bytes);
[[nodiscard]] nonstd::expected<Instructions, ParsingError> parseInstructions(std::span<std::uint8_t>& bytes);
}
