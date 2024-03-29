#ifndef MINI_JVM_LIB_TYPES_CONST_POOL_HPP
#define MINI_JVM_LIB_TYPES_CONST_POOL_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <variant>
#include <vector>
#include <stdexcept>

#include <fmt/format.h>

#include "jvm/types/basic.hpp"
#include "jvm/types/class/base.hpp"
#include "jvm/utils/ref.hpp"

namespace jvm {

class ConstPool {
public:
    enum class Tag : std::uint8_t {
        String = 0x01,
        Integer = 0x03,
        Float = 0x04,
        Long = 0x05,
        Double = 0x06,
        Class = 0x07,
        StringRef = 0x08,
        FieldRef = 0x09,
        MethodRef = 0x0a,
        InterfaceMethodRef = 0x0b,
        NameAndType = 0x0c,
        MethodHandle = 0x0f,
        MethodType = 0x10,
        Dynamic = 0x11,
        InvokeDynamic = 0x12,
        Module = 0x13,
        Package = 0x14
    };

    struct NameAndType {
        Index name;
        Index type;
    };

    struct MethodRef {
        Index classInfo;
        Index nameAndType;
    };

    struct FieldRef {
        Index classInfo;
        Index nameAndType;
    };

    struct StringRef {
        Index string;
    };

    struct ClassInfo {
        Index name;
    };

    struct InvokeDynamic {
        Index boostrapMethod;
        Index nameAndType;
    };

    struct MethodHandle {
        enum class Type: std::uint8_t {
            GET_FIELD = 1,
            GET_STATIC = 2,
            PUT_FIELD = 3,
            PUT_STATIC = 4,
            INVOKE_VIRTUAL = 5,
            INVOKE_STATIC = 6,
            INVOKE_SPECIAL = 7,
            NEW_INVOKE_SPECIAL = 8,
            INVOKE_INTERFACE = 9
        };
        Type kind;
        Index referenceIndex;
    };

    struct Error {
        std::string message;
    };

    using Value = std::variant<ClassInfo, StringRef, FieldRef, MethodRef, NameAndType, std::string, InvokeDynamic, MethodHandle>;

    static nonstd::expected<ConstPool, ParsingError> load(std::istream& in);

    nonstd::expected<jvm::ConstRef<Value>, Error> operator[](const std::size_t index) const;

    template<typename Type>
    nonstd::expected<ConstRef<Type>, Error> getRef(const std::size_t index) const {
        auto res = validateIndex(index);
        PROPAGATE_ERROR(res);
        auto& object = data[index - 1];
        if (std::holds_alternative<Type>(object)) {
            return const_cast<Type &>(std::get<Type>(object));
        }
        return nonstd::make_unexpected(
            Error{fmt::format("Const pool value at index {}, doesn't hold type: {}", index, typeid(Type).name())}
        );
    }

    std::size_t size() const;

protected:
    std::vector<Value> data;
    std::size_t dataSize;

    [[nodiscard]] nonstd::expected<void, ParsingError> loadInstance(std::istream& in);
    [[nodiscard]] nonstd::expected<void, Error> validateIndex(const std::size_t index) const;

    template<typename Type>
    nonstd::expected<Type&, Error> get(const std::size_t index) {
        auto res = validateIndex(index);
        PROPAGATE_ERROR(res);
        auto& object = data[index - 1];
        if (std::holds_alternative<Type>(object)) {
            return std::get<Type>(object);
        }
        return nonstd::make_unexpected(
            Error{fmt::format("Const pool value at index {}, doesn't hold type: {}", index, typeid(Type).name())}
        );
    }
};

}

#endif//MINI_JVM_LIB_TYPES_CONST_POOL_HPP
