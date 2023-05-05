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

TEST_CASE("Test int loading instructions", "[ops]") {
    jvm::Stack stack{};

    test<int, jvm::op::iconst_0>(stack, 1, 0);
    test<int, jvm::op::iconst_1>(stack, 2, 1);
    test<int, jvm::op::iconst_2>(stack, 3, 2);
    test<int, jvm::op::iconst_3>(stack, 4, 3);
    test<int, jvm::op::iconst_4>(stack, 5, 4);
    test<int, jvm::op::iconst_5>(stack, 6, 5);
    test<int, jvm::op::iconst_m1>(stack, 7, -1);
}

TEST_CASE("Test null loading instruction", "[ops]") {
    jvm::Stack stack{};
    test<std::monostate, jvm::op::aconst_null>(stack, 1, std::monostate{});
}

TEST_CASE("Test long loading instructions", "[ops]") {
    jvm::Stack stack{};
    
    test<std::int64_t, jvm::op::lconst_0>(stack, 1, 0L);
    test<std::int64_t, jvm::op::lconst_1>(stack, 2, 1L);
}

template<typename InstructionType>
void testipush(jvm::Stack& stack, std::vector<std::uint8_t> bytes, std::size_t expectedStackSize, int expectedValue) {
    jvm::Bytes bytesView(bytes.data(), bytes.size());
    auto instruction = InstructionType::parse(bytesView);
    instruction(stack);
    REQUIRE(stack.size() == expectedStackSize);
    auto top = stack.top();
    REQUIRE(std::holds_alternative<int>(top));
    REQUIRE(std::get<int>(top) == expectedValue);
}

TEST_CASE("Test pushing constatnt from smaller type", "[ops]") {
    jvm::Stack stack{};

    testipush<jvm::op::bipush>(stack, {0x00}, 1, 0);
    testipush<jvm::op::bipush>(stack, {0x69}, 2, 0x69);
    testipush<jvm::op::sipush>(stack, {0x00, 0x00}, 3, 0);
    testipush<jvm::op::sipush>(stack, {0x00, 0x12}, 4, 0x12);
    testipush<jvm::op::sipush>(stack, {0x16, 0x00}, 5, 0x1600);
}
