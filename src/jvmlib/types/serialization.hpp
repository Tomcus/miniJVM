#ifndef MINI_JVM_TYPES_SERIALIZATION_HPP
#define MINI_JVM_TYPES_SERIALIZATION_HPP

#include <iostream>
#include <string_view>
#include <array>
#include <concepts>
#include <stdexcept>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace jvm {

constexpr static int ISTREAM_EOF_ERROR = 100;
constexpr static int ISTREAM_FAIL_ERROR = 101;
constexpr static int ISTREAM_BAD_ERROR = 102;


inline int getIStreamErrorNumber(const std::istream& in) {
    if (in.eof()) {
        return ISTREAM_EOF_ERROR;
    }
    if (in.fail()) {
        return ISTREAM_FAIL_ERROR;
    }
    if (in.bad()) {
        return ISTREAM_BAD_ERROR;
    }
    return -1;
}

inline const std::string_view getIStreamErrorString(const std::istream& in) {
    if (in.eof()) {
        return "End of file reached.";
    }
    if (in.fail()) {
        return "Rocoverable error reached. Wrong data format read.";
    }
    if (in.bad()) {
        return "Unrecoverable error reached.";
    }
    return "No error";
}

template<typename ReturnType>
ReturnType read(std::istream&) {
    throw std::runtime_error{"Unsupported type to read."};
}

template<std::unsigned_integral ReturnType> 
ReturnType read(std::istream& in) {
    static std::array<char, 64> readBuffer{};
    if (!in.read(readBuffer.data(), sizeof(ReturnType))) {
        throw std::runtime_error{fmt::format("Can't read the unsigned integer of size {} bytes. {}", sizeof(ReturnType), getIStreamErrorString(in))};
    }
    ReturnType res = 0;
    for (std::size_t i = 0; i < sizeof(ReturnType); ++i) {
        res = res << 8;
        res += static_cast<std::uint8_t>(readBuffer[i]);
    }
    spdlog::debug("Read {}-bytes unsigned integer with value: {:#04x}", sizeof(ReturnType), res);
    return res;
}

}

#endif//MINI_JVM_TYPES_SERIALIZATION_HPP