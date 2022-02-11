#ifndef MINI_JVM_TYPES_CLASS_HPP
#define MINI_JVM_TYPES_CLASS_HPP

#include "basic.hpp"
#include "const_pool.hpp"

#include <cstdint>
#include <stdexcept>
#include <string_view>
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
    constexpr static std::uint16_t MIN_CLASS_FILE_FORMAT_MAJOR_VERSION = 55;

    enum class AccessFlags: std::uint16_t {
        ACC_PUBLIC = 0x001,
        ACC_FINAL = 0x010,
        ACC_SUPER = 0x020,
        ACC_INTERFACE = 0x200,
        ACC_ABSTRACT = 0x400
    };

    static Class load(const std::filesystem::path& path);

    std::string_view getClassName() const;
    std::string_view getParentClassName() const;

    ver16 version;
    std::vector<ConstPool::Value> constPool;
    AccessFlags accessFlags;
    ConstPool::Index thisClass;
    ConstPool::Index superClass;
    Interfaces interfaces;
    Fields fields;
    Fields methods;
    Attributes attributes;

protected:

    void readVersion(std::istream& in);
    void readConstPool(std::istream& in);
    void readInterfaces(std::istream& in);
};

class ReadingError: public std::runtime_error {
public:
    ReadingError(const std::string& message): runtime_error(message) { }
};

class ClassError: public std::runtime_error {
public:
    ClassError(const std::string& message): runtime_error(message) { }
};

}

#endif//MINI_JVM_TYPES_CLASS_HPP