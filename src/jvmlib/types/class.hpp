#ifndef MINI_JVM_TYPES_CLASS_HPP
#define MINI_JVM_TYPES_CLASS_HPP

#include "basic.hpp"
#include "const_pool.hpp"
#include "class/attribute.hpp"
#include "class/field.hpp"
#include "class/method.hpp"
#include "utils/ref.hpp"

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

using Interfaces = std::vector<ConstRef<ConstPool::ClassInfo>>;

class Class {
public:
    constexpr static std::uint16_t MIN_CLASS_FILE_FORMAT_MAJOR_VERSION = 55;

    static Class load(const std::filesystem::path& path);

    std::string_view getClassName() const;
    std::string_view getParentClassName() const;

    ver16 version;
    ConstPool constPool;
    AccessFlags accessFlags;
    ConstRef<ConstPool::ClassInfo> thisClass;
    ConstRef<ConstPool::ClassInfo> superClass;
    Interfaces interfaces;
    Fields fields;
    Methods methods;
    Attributes attributes;

protected:

    void readVersion(std::istream& in);
    void readInterfaces(std::istream& in);
    void readFields(std::istream& in);
    void readMethods(std::istream& in);
    Attributes readAttributes(std::istream& in) const;
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