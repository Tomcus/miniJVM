#include "jvm/execution/instructions/int_ops.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test int add", "[ops]") {
    jvm::Stack stack{};
    stack.push(1);
    stack.push(2);
    auto op = jvm::op::iadd::parse();
    op(stack);
    REQUIRE(stack.size() == 1);
    REQUIRE(std::holds_alternative<int>(stack.top()));
    REQUIRE(std::get<int>(stack.top()) == 3);
}
