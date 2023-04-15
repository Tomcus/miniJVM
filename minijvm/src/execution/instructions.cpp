#include "jvm/execution/instructions.hpp"

#include <optional>

#include <fmt/format.h>

nonstd::expected<jvm::Instruction, jvm::ParsingError> jvm::parseInstruction(std::span<std::uint8_t>& bytes) {
    if (bytes.size() == 0) {
        return nonstd::make_unexpected(ParsingError{
            .message = "Unable to parse instruction: bytes are empty"
        });
    }

    auto opCode = bytes[0];
    bytes = bytes.subspan(1);
    switch(opCode) {
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
        default:
            return nonstd::make_unexpected(ParsingError{
                .message = fmt::format("Parsing unhandeled instruction with opcode: {:02x}", bytes[0])
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
