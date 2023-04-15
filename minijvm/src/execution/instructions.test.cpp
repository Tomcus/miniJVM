#include <catch2/catch_test_macros.hpp>

#include "jvm/execution/instructions.hpp"

#include <vector>
#include <cstdint>

TEST_CASE("Test instruction parsing - single", "[ops]") {
    std::vector<std::uint8_t> bytes = {
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
    };

    auto span = std::span{bytes.data(), bytes.size()};
    auto res = jvm::parseInstructions(span);
    
    REQUIRE(res.has_value());
    REQUIRE(span.size() == 0);
    auto instructions = *res;
    REQUIRE(instructions.size() == 7);

    // iconst instructions
    REQUIRE(std::holds_alternative<jvm::op::iconst_m1>(instructions[0]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_0>(instructions[1]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_1>(instructions[2]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_2>(instructions[3]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_3>(instructions[4]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_4>(instructions[5]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_5>(instructions[6]));
}
