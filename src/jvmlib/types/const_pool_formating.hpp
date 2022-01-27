#ifndef MINI_JVM_CONST_POOL_FORMATING
#define MINI_JVM_CONST_POOL_FORMATING

#include "basic_formating.hpp"
#include "const_pool.hpp"
#include <string>
#include <fmt/format.h>
#include <variant>

namespace jvm {

template<>
inline std::string format(const ConstPool::NameAndType& nameAndType) noexcept {
    return fmt::format("ConstPool::NameAndType(.name={:#04x}, .type={:#04x})", nameAndType.name, nameAndType.type);
}

template<>
inline std::string format(const ConstPool::MethodRef& mr) noexcept {
    return fmt::format("ConstPool::MethodRef(.class={:#04x}, .nameAndType={:#04x})", mr.classInfo, mr.nameAndType);
}

template<>
inline std::string format(const ConstPool::ClassInfo& ci) noexcept {
    return fmt::format("ConstPool::ClassInfo(.name={:#04x})", ci.name);
}

}

#endif//MINI_JVM_CONST_POOL_FORMATING