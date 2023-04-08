#ifndef MINI_JVM_TYPES_SERIALIZATION_HPP
#define MINI_JVM_TYPES_SERIALIZATION_HPP

#include <iostream>
#include <string_view>
#include <array>
#include <concepts>
#include <stdexcept>

#include <fmt/format.h>
#include <fmtlog/fmtlog.h>
#include <nonstd/expected.hpp>

#include "jvm/types/basic.hpp"

namespace jvm {

template<typename ReturnType>
nonstd::expected<ReturnType, ParsingError> read(std::istream&) {
    return nonstd::make_unexpected(ParsingError::unsuported_type());
}

template<std::unsigned_integral ReturnType> 
nonstd::expected<ReturnType, ParsingError> read(std::istream& in) {
    static std::array<char, 64> readBuffer{};
    if (!in.read(readBuffer.data(), sizeof(ReturnType))) {
        return nonstd::make_unexpected(ParsingError::cant_read_from_istream(in, fmt::format("unsigned integer of size {}", sizeof(ReturnType))));
    }
    ReturnType res = 0;
    for (std::size_t i = 0; i < sizeof(ReturnType); ++i) {
        res = res << 8;
        res += static_cast<std::uint8_t>(readBuffer[i]);
    }
    logd("Read {}-bytes unsigned integer with value: {:#04x}", sizeof(ReturnType), res);
    return res;
}

}

#endif//MINI_JVM_TYPES_SERIALIZATION_HPP
