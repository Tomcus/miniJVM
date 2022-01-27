#ifndef MINI_JVM_TYPES_BASIC_FORMATING_HPP
#define MINI_JVM_TYPES_BASIC_FORMATING_HPP

#include "basic.hpp"
#include <cstdint>
#include <string>

namespace jvm {

template<typename JvmType>
inline std::string format(const JvmType&) noexcept {
    return "Unknown jvm type to format";
}

template<>
inline std::string format(const int& val) noexcept {
    return std::to_string(val);
}

template<>
inline std::string format(const float& val) noexcept {
    return std::to_string(val);
}

template<>
inline std::string format(const std::int64_t& val) noexcept {
    return std::to_string(val);
}

template<>
inline std::string format(const double& val) noexcept {
    return std::to_string(val);
}

template<>
inline std::string format(const std::string& val) noexcept {
    return val;
}

}

#endif//MINI_JVM_TYPES_BASIC_FORMATING_HPP