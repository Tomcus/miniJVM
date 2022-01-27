#ifndef MINI_JVM_TYPES_CONST_POOL_VALIDATION
#define MINI_JVM_TYPES_CONST_POOL_VALIDATION

#include "const_pool.hpp"
#include "error/error.hpp"

namespace jvm::ConstPool {

template<typename ConstPoolType>
Error validate(const ConstPoolType&, const std::vector<ConstPool::Value>&) {
    return Error{"Unknown type to validate"};
}

template<>
Error validate(const BasicType&, const std::vector<ConstPool::Value>&) {
    return {};
}

template<>
Error validate(const ClassInfo& ci, const std::vector<ConstPool::Value>& cp) {
    CHECK(ci.name > 0);
    CHECK(ci.name <= cp.size());
    CHECK(std::holds_alternative<BasicType>(cp[ci.name - 1]));
    const auto& referencedName = std::get<BasicType>(cp[ci.name - 1]);
    CHECK(std::holds_alternative<std::string>(referencedName));
    return {};
}

template<>
Error validate(const NameAndType& nat, const std::vector<ConstPool::Value>& cp) {
    CHECK(nat.name > 0);
    CHECK(nat.name <= cp.size());
    CHECK(nat.type > 0);
    CHECK(nat.type <= cp.size());
    CHECK(std::holds_alternative<BasicType>(cp[nat.name - 1]));
    const auto& nameReference = std::get<BasicType>(cp[nat.name - 1]);
    CHECK(std::holds_alternative<std::string>(nameReference));
    CHECK(std::holds_alternative<BasicType>(cp[nat.type - 1]));
    const auto& typeReference = std::get<BasicType>(cp[nat.type - 1]);
    CHECK(std::holds_alternative<std::string>(typeReference));
    return {};
}

template<>
Error validate(const MethodRef& mr, const std::vector<ConstPool::Value>& cp) {
    CHECK(mr.classInfo > 0);
    CHECK(mr.classInfo <= cp.size());
    CHECK(mr.nameAndType > 0);
    CHECK(mr.nameAndType <= cp.size());
    CHECK(std::holds_alternative<ClassInfo>(cp[mr.classInfo - 1]));
    CHECK(std::holds_alternative<NameAndType>(cp[mr.nameAndType - 1]));
    return {};
}
}

#endif