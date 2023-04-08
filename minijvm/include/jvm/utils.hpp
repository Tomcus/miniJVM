#ifndef MINI_JVM_LIB_UTILS_HPP
#define MINI_JVM_LIB_UTILS_HPP

#include <string_view>
#include <typeinfo>

#include <fmt/format.h>
#include <nonstd/expected.hpp>

namespace jvm {

template<typename Error>
inline nonstd::expected<void, Error> check(bool condition, std::string_view message = "NO MESSAGE PROVIDED") {
    if (!condition) {
        return nonstd::make_unexpected(Error{fmt::format("Check failed with message: {}", message)});
    }
    return {};
}

}

#endif//MINI_JVM_LIB_UTILS_HPP
