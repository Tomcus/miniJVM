#include <catch2/catch_test_macros.hpp>

#include "jvm/execution/instructions/misc.hpp"

TEST_CASE("Test nop", "[ops]") {
    jvm::Stack stack{};

    jvm::op::nop nopInst{};
    nopInst(stack);

    REQUIRE(stack.size() == 0);
}
