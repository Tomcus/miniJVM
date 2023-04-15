#include <catch2/catch_test_macros.hpp>

#include "jvm/execution/instructions/stack_load_const.hpp"

template <typename PrimitiveType, typename InstructionType>
void test(jvm::Stack& stack, std::size_t expectedStackSize, PrimitiveType expectedStoredValue) {
    auto instruction = InstructionType::parse();
    instruction(stack);
    REQUIRE(stack.size() == expectedStackSize);
    auto top = stack.top();
    REQUIRE(std::holds_alternative<PrimitiveType>(top));
    REQUIRE(std::get<PrimitiveType>(top) == expectedStoredValue);
}

TEST_CASE("Test int loading instruction", "[ops]") {
    jvm::Stack stack{};

    test<int, jvm::op::iconst_0>(stack, 1, 0);
    test<int, jvm::op::iconst_1>(stack, 2, 1);
    test<int, jvm::op::iconst_2>(stack, 3, 2);
    test<int, jvm::op::iconst_3>(stack, 4, 3);
    test<int, jvm::op::iconst_4>(stack, 5, 4);
    test<int, jvm::op::iconst_5>(stack, 6, 5);
    test<int, jvm::op::iconst_m1>(stack, 7, -1);
}
