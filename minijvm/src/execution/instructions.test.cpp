#include <catch2/catch_test_macros.hpp>

#include "jvm/execution/instructions.hpp"

#include <vector>
#include <cstdint>
#include <fmtlog/fmtlog.h>

TEST_CASE("Test instruction parsing", "[ops]") {
    std::vector<std::uint8_t> bytes = {
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x01,
        0x00,
        0x2a,
        0x2b,
        0x2c,
        0x2d,
        0x19, 0x00, 0x00,
        0x19, 0x00, 0x05,
        0x19, 0x01, 0x00,
        0xb7, 0x00, 0x15,
        0xb7, 0x02, 0x22,
        0xb1,
        0x09,
        0x0a,
        0x1a,
        0x1b,
        0x1c,
        0x1d,
        0x15, 0x00, 0x69,
        0x15, 0x69, 0x00,
        0x60,
        0xac,
        0x10, 0x00,
        0x10, 0x01,
        0x11, 0x12, 0x34,
        0x12, 0x69,
        0x13, 0x12, 0x34,
        0x14, 0x56, 0x78
    };

    auto span = std::span{bytes.data(), bytes.size()};
    auto res = jvm::parseInstructions(span);
   
    REQUIRE(res);
    REQUIRE(span.size() == 0);
    auto instructions = *res;
    REQUIRE(instructions.size() == 35);

    // iconst instructions
    REQUIRE(std::holds_alternative<jvm::op::iconst_m1>(instructions[0]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_0>(instructions[1]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_1>(instructions[2]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_2>(instructions[3]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_3>(instructions[4]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_4>(instructions[5]));
    REQUIRE(std::holds_alternative<jvm::op::iconst_5>(instructions[6]));
    REQUIRE(std::holds_alternative<jvm::op::aconst_null>(instructions[7]));
    REQUIRE(std::holds_alternative<jvm::op::nop>(instructions[8]));
    REQUIRE(std::holds_alternative<jvm::op::aload_0>(instructions[9]));
    REQUIRE(std::holds_alternative<jvm::op::aload_1>(instructions[10]));
    REQUIRE(std::holds_alternative<jvm::op::aload_2>(instructions[11]));
    REQUIRE(std::holds_alternative<jvm::op::aload_3>(instructions[12]));
    
    REQUIRE(std::holds_alternative<jvm::op::aload>(instructions[13]));
    REQUIRE(std::get<jvm::op::aload>(instructions[13]).localIndex == 0);
    REQUIRE(std::holds_alternative<jvm::op::aload>(instructions[14]));
    REQUIRE(std::get<jvm::op::aload>(instructions[14]).localIndex == 5);
    REQUIRE(std::holds_alternative<jvm::op::aload>(instructions[15]));
    REQUIRE(std::get<jvm::op::aload>(instructions[15]).localIndex == 0x100);
    
    REQUIRE(std::holds_alternative<jvm::op::invokespecial>(instructions[16]));
    REQUIRE(std::get<jvm::op::invokespecial>(instructions[16]).cpIndex == 0x15);
    REQUIRE(std::holds_alternative<jvm::op::invokespecial>(instructions[17]));
    REQUIRE(std::get<jvm::op::invokespecial>(instructions[17]).cpIndex == 0x222);
    
    REQUIRE(std::holds_alternative<jvm::op::ret>(instructions[18]));
    REQUIRE(std::holds_alternative<jvm::op::lconst_0>(instructions[19]));
    REQUIRE(std::holds_alternative<jvm::op::lconst_1>(instructions[20]));
    REQUIRE(std::holds_alternative<jvm::op::iload_0>(instructions[21]));
    REQUIRE(std::holds_alternative<jvm::op::iload_1>(instructions[22]));
    REQUIRE(std::holds_alternative<jvm::op::iload_2>(instructions[23]));
    REQUIRE(std::holds_alternative<jvm::op::iload_3>(instructions[24]));
    
    REQUIRE(std::holds_alternative<jvm::op::iload>(instructions[25]));
    REQUIRE(std::get<jvm::op::iload>(instructions[25]).localIndex == 0x69);
    REQUIRE(std::holds_alternative<jvm::op::iload>(instructions[26]));
    REQUIRE(std::get<jvm::op::iload>(instructions[26]).localIndex == 0x6900);
    
    REQUIRE(std::holds_alternative<jvm::op::iadd>(instructions[27]));
    REQUIRE(std::holds_alternative<jvm::op::ireturn>(instructions[28]));

    REQUIRE(std::holds_alternative<jvm::op::bipush>(instructions[29]));
    REQUIRE(std::get<jvm::op::bipush>(instructions[29]).toPush == 0);
    REQUIRE(std::holds_alternative<jvm::op::bipush>(instructions[30]));
    REQUIRE(std::get<jvm::op::bipush>(instructions[30]).toPush == 1);
    REQUIRE(std::holds_alternative<jvm::op::sipush>(instructions[31]));
    REQUIRE(std::get<jvm::op::sipush>(instructions[31]).toPush == 0x1234);

    REQUIRE(std::holds_alternative<jvm::op::ldc>(instructions[32]));
    REQUIRE(std::get<jvm::op::ldc>(instructions[32]).cpIndex == 0x69);
    REQUIRE(std::holds_alternative<jvm::op::ldc_w>(instructions[33]));
    REQUIRE(std::get<jvm::op::ldc_w>(instructions[33]).cpIndex == 0x1234);
    REQUIRE(std::holds_alternative<jvm::op::ldc2_w>(instructions[34]));
    REQUIRE(std::get<jvm::op::ldc2_w>(instructions[34]).cpIndex == 0x5678);

    fmtlog::poll();
}
