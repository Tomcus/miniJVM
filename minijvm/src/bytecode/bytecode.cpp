#include <jvm/bytecode/bytecode.hpp>

namespace jvm::bytecode {

Instructions parse(std::istream& input, std::uint16_t bytesCount) {
    Instructions res{};
    auto start = input.tellg();
    while(input.tellg() - start < bytesCount) {
        std::uint8_t instructionNumber;
        input.read(reinterpret_cast<char*>(&instructionNumber), 1);
        switch (instructionNumber) {
            case 0x0:
                res.push_back(nop{});
                break;
            case 0x1:
                res.push_back(aconst_null{});
                break;
            default:
                // TODO: throw a non implemented error
            break;
        }
    }
    // TODO: Check if loaded exactly n bytes
    return res;
}

}
