#ifndef MINI_JVM_TYPES_CLASS_HPP
#define MINI_JVM_TYPES_CLASS_HPP

#include "basic.hpp"
#include "const_pool.hpp"
#include "error/error.hpp"

#include <cstdint>
#include <vector>
#include <filesystem>

namespace jvm {

struct ver16 {
    std::uint16_t major;
    std::uint16_t minor;
};

struct Attribute {
    ConstPool::Index attrNameIndex;
    std::vector<Byte> data;
};

using Attributes = std::vector<Attribute>;

struct Field {
    std::uint16_t flags;
    ConstPool::Index nameIndex;
    ConstPool::Index descriptorIndex;
    Attributes attributes;
};

using Fields = std::vector<jvm::Field>;
using Interfaces = std::vector<ConstPool::Index>;

class Class {
public:
    enum class AccessFlags: std::uint16_t {
        ACC_PUBLIC = 0x001,
        ACC_FINAL = 0x010,
        ACC_SUPER = 0x020,
        ACC_INTERFACE = 0x200,
        ACC_ABSTRACT = 0x400
    };

    static ErrorOr<Class> load(const std::filesystem::path& path) noexcept;
protected:

    Error readVersion(std::istream& in) noexcept;
    Error readConstPool(std::istream& in) noexcept;

    ver16 version;
    std::vector<ConstPool::Value> constPool;
    AccessFlags accessFlags;
    ConstPool::Index thisClass;
    ConstPool::Index superClass;
    Interfaces interfaces;
    Fields fields;
    Fields methods;
    Attributes attributes;
};

}

#endif//MINI_JVM_TYPES_CLASS_HPP