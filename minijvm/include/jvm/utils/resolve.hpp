#ifndef MINI_JVM_LIB_UTILS_RESOLVE_HPP
#define MINI_JVM_LIB_UTILS_RESOLVE_HPP

#include <variant>
#include <stdexcept>

namespace jvm {

class ResolveError: std::runtime_error {
public:
    ResolveError(const std::string& message): runtime_error(message) { }
};

template<typename SubType, typename Variant>
inline SubType& resolve(Variant& value) {
    if (std::holds_alternative<SubType>(value)) {
        return std::get<SubType>(value);
    }
    throw ResolveError{"Unable to resolve value. Wrong subtype."};
}

template<typename SubType, typename Variant>
inline const SubType& resolve(const Variant& value) {
    if (std::holds_alternative<SubType>(value)) {
        return std::get<SubType>(value);
    }
    throw ResolveError{"Unable to resolve value. Wrong subtype."};
}

}

#endif//MINI_JVM_LIB_UTILS_RESOLVE_HPP