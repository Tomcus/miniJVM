#include "jvm/execution/instructions.hpp"

#include <optional>

#include <fmt/format.h>
#include <fmtlog/fmtlog.h>

nonstd::expected<jvm::Instruction, jvm::ParsingError> jvm::parseInstruction(std::span<std::uint8_t>& bytes) {
    if (bytes.size() == 0) {
        return nonstd::make_unexpected(ParsingError{
            .message = "Unable to parse instruction: bytes are empty"
        });
    }

    std::uint8_t opCode = bytes[0];
    logd("Parsing: {:02x}", opCode);
    bytes = bytes.subspan(1);
    switch(opCode) {
        case op::nop::OP_CODE:
            return op::nop::parse();
        case op::aconst_null::OP_CODE:
            return op::aconst_null::parse();
        case op::iconst_m1::OP_CODE: 
            return op::iconst_m1::parse(); 
        case op::iconst_0::OP_CODE: 
            return op::iconst_0::parse(); 
        case op::iconst_1::OP_CODE: 
            return op::iconst_1::parse(); 
        case op::iconst_2::OP_CODE: 
            return op::iconst_2::parse(); 
        case op::iconst_3::OP_CODE: 
            return op::iconst_3::parse(); 
        case op::iconst_4::OP_CODE: 
            return op::iconst_4::parse(); 
        case op::iconst_5::OP_CODE: 
            return op::iconst_5::parse(); 
        case op::lconst_0::OP_CODE: 
            return op::lconst_0::parse(); 
        case op::lconst_1::OP_CODE: 
            return op::lconst_1::parse(); 
        case op::aload_0::OP_CODE: 
            return op::aload_0::parse(); 
        case op::aload_1::OP_CODE: 
            return op::aload_1::parse(); 
        case op::aload_2::OP_CODE: 
            return op::aload_2::parse(); 
        case op::aload_3::OP_CODE: 
            return op::aload_3::parse(); 
        case op::aload::OP_CODE:
            return op::aload::parse(bytes);
        case op::invokespecial::OP_CODE:
            return op::invokespecial::parse(bytes);
        case op::ret::OP_CODE:
            return op::ret::parse();
        case op::iload::OP_CODE:
            return op::iload::parse(bytes);
        case op::iload_0::OP_CODE:
            return op::iload_0::parse();
        case op::iload_1::OP_CODE:
            return op::iload_1::parse();
        case op::iload_2::OP_CODE:
            return op::iload_2::parse();
        case op::iload_3::OP_CODE:
            return op::iload_3::parse();
        case op::iadd::OP_CODE:
            return op::iadd::parse();
        case op::ireturn::OP_CODE:
            return op::ireturn::parse();
        case op::bipush::OP_CODE:
            return op::bipush::parse(bytes);
        case op::sipush::OP_CODE:
            return op::sipush::parse(bytes);
        default:
            return nonstd::make_unexpected(ParsingError{
                .message = fmt::format("Parsing unhandeled instruction with opcode: {:02x}", opCode)
            });
    }
}

nonstd::expected<jvm::Instructions, jvm::ParsingError> jvm::parseInstructions(std::span<std::uint8_t>& bytes) {
    jvm::Instructions res{};

    while (!bytes.empty()) {
        auto insOrErr = jvm::parseInstruction(bytes);
        PROPAGATE_ERROR(insOrErr);
        auto ins = *insOrErr;
        res.push_back(ins);
    }

    return res;
}
