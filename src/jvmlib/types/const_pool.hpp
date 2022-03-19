#ifndef MINI_JVM_LIB_TYPES_CONST_POOL_HPP
#define MINI_JVM_LIB_TYPES_CONST_POOL_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <variant>
#include <vector>
#include <stdexcept>

#include <fmt/format.h>

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

    using Index = std::uint16_t;

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

    using Value = std::variant<ClassInfo, StringRef, FieldRef, MethodRef, NameAndType, std::string, InvokeDynamic, MethodHandle>;

    class Error: public std::runtime_error {
    public:
        Error(const std::string& message): runtime_error(message) { }
    };

    static ConstPool load(std::istream& in);

    const Value& operator[](const std::size_t index) const;

    template<typename Type>
    const Type& get(const std::size_t index) const {
        validateIndex(index);
        auto& object = data[index - 1];
        if (std::holds_alternative<Type>(object)) {
            return std::get<Type>(object);
        }
        throw Error{fmt::format("Const pool value at index {}, doesn't hold type: {}", index, typeid(Type).name())};
    }

    std::size_t size() const;

protected:
    std::vector<Value> data;
    std::size_t dataSize;

    void loadInstance(std::istream& in);
    void validateIndex(const std::size_t index) const;

    template<typename Type>
    Type& get(const std::size_t index) {
        validateIndex(index);
        auto& object = data[index - 1];
        if (std::holds_alternative<Type>(object)) {
            return std::get<Type>(object);
        }
        throw Error{fmt::format("Const pool value at index {}, doesn't hold type: {}", index, typeid(Type).name())};
    }
};

}

#endif//MINI_JVM_LIB_TYPES_CONST_POOL_HPP