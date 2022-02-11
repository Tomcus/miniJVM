#ifndef MINI_JVM_TYPES_CONST_POOL_VALIDATION
#define MINI_JVM_TYPES_CONST_POOL_VALIDATION

#include "const_pool.hpp"
#include <stdexcept>

namespace jvm::ConstPool {

template<typename ConstPoolType>
void validate(const ConstPoolType&, const std::vector<ConstPool::Value>&) {
    throw std::runtime_error{"Unknown type to validate"};
}

template<>
inline void validate(const BasicType&, const std::vector<ConstPool::Value>&) { }

template<>
inline void validate(const ClassInfo& ci, const std::vector<ConstPool::Value>& cp) {
    check<Error>(ci.name > 0);
    check<Error>(ci.name <= cp.size());
    check<Error>(std::holds_alternative<BasicType>(cp[ci.name - 1]));
    const auto& referencedName = std::get<BasicType>(cp[ci.name - 1]);
    check<Error>(std::holds_alternative<std::string>(referencedName));
}

template<>
inline void validate(const NameAndType& nat, const std::vector<ConstPool::Value>& cp) {
    check<Error>(nat.name > 0);
    check<Error>(nat.name <= cp.size());
    check<Error>(nat.type > 0);
    check<Error>(nat.type <= cp.size());
    check<Error>(std::holds_alternative<BasicType>(cp[nat.name - 1]));
    const auto& nameReference = std::get<BasicType>(cp[nat.name - 1]);
    check<Error>(std::holds_alternative<std::string>(nameReference));
    check<Error>(std::holds_alternative<BasicType>(cp[nat.type - 1]));
    const auto& typeReference = std::get<BasicType>(cp[nat.type - 1]);
    check<Error>(std::holds_alternative<std::string>(typeReference));
}

template<>
inline void validate(const MethodRef& mr, const std::vector<ConstPool::Value>& cp) {
    check<Error>(mr.classInfo > 0);
    check<Error>(mr.classInfo <= cp.size());
    check<Error>(mr.nameAndType > 0);
    check<Error>(mr.nameAndType <= cp.size());
    check<Error>(std::holds_alternative<ClassInfo>(cp[mr.classInfo - 1]));
    check<Error>(std::holds_alternative<NameAndType>(cp[mr.nameAndType - 1]));
}
}

#endif