#ifndef MINI_JVM_LIB_UTILS_HPP
#define MINI_JVM_LIB_UTILS_HPP

#include <string_view>
#include <typeinfo>

#include <fmt/format.h>

namespace jvm {

template<typename Error>
inline void check(bool condition, const std::string_view message = "") {
    if (!condition) {
        if (message.empty()) {
            throw Error{fmt::format("Check failed with message: NO MESSAGE PROVIDED (err_type: {})", typeid(Error).name())};
        } else {
            throw Error{fmt::format("Check failed with message: {}", message)};
        }
    }
}

}

#endif//MINI_JVM_LIB_UTILS_HPP